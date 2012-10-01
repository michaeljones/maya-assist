
#include "CommandSpec.hh"

#include <maya/MPxCommand.h>
#include <maya/MArgParser.h>

#include <json/json.h>

#include <sstream>
#include <list>

namespace ma {
    
CommandSpec::CommandSpec( const char* name, const char* arguments, const char* description )
 : m_name( name ),
   m_arguments( arguments ),
   m_description( description ),
   m_syntax()
{
    const char* kHelp = "Help";
    addFlag(
            "h",
            "help",
            kHelp,
            commandproperties::kCreate,
            "Shows the help"
           );

    addFlag(
            "hj",
            "helpJson",
            kHelp,
            commandproperties::kCreate,
            "Returns the help in a json structure"
           );
}

MSyntax CommandSpec::syntax()
{
    return m_syntax;
}

bool CommandSpec::handle( const MArgList& args )
{
    MArgParser argParser( m_noArgsSyntax, args );

    if ( argParser.isFlagSet( "-h" ) || argParser.isFlagSet( "-help" ) )
    {
        std::stringstream output;

        output << "//" << std::endl;
        output << "//   " << m_name << m_arguments << std::endl;
        output << "//" << std::endl;
        output << "//   " << m_description << std::endl;
        output << "//" << std::endl;

        std::vector< std::string >::iterator categoryIt = m_categories.begin();
        std::vector< std::string >::iterator categoryEnd = m_categories.end();

        for ( ; categoryIt != categoryEnd; ++categoryIt )
        {
            output << "//   " << *categoryIt << ":" << std::endl;
            output << "//" << std::endl;
            for ( size_t i=0; i<m_flags.size(); ++i )
            {
                Flag& flag = m_flags[ i ];
                if ( flag.category == *categoryIt )
                {
                    output << "//     -" << flag.shortName << "/-" << flag.longName << std::endl;
                    output << "//       " << flag.description << std::endl;
                    output << "//" << std::endl;
                }
            }
        }

        MPxCommand::displayInfo( output.str().c_str() );
        return true;
    }
    else if ( argParser.isFlagSet( "-hj" ) || argParser.isFlagSet( "-helpJson" ) )
    {
        Json::Value jsonRoot;
        jsonRoot[ "name" ] = m_name;
        jsonRoot[ "description" ] = m_description;
        jsonRoot[ "arguments" ] = m_arguments;

        Json::Value& jsonCategories = jsonRoot[ "categories" ];

        std::vector< std::string >::iterator categoryIt = m_categories.begin();
        std::vector< std::string >::iterator categoryEnd = m_categories.end();

        for ( ; categoryIt != categoryEnd; ++categoryIt )
        {
            jsonCategories.append( *categoryIt );
        }

        Json::Value& jsonFlags = jsonRoot[ "flags" ];

        for ( size_t i=0; i<m_flags.size(); ++i )
        {
            Flag& flag = m_flags[ i ];
            Json::Value jsonFlag;

            jsonFlag[ "shortname" ] = flag.shortName;
            jsonFlag[ "longname" ] = flag.longName;
            jsonFlag[ "description" ] = flag.description;
            jsonFlag[ "category" ] = flag.category;

            Json::Value jsonArguments = Json::Value( Json::arrayValue );

            std::vector< std::string >::iterator argumentIt = flag.arguments.begin();
            std::vector< std::string >::iterator argumentEnd = flag.arguments.end();

            for ( ; argumentIt != argumentEnd; ++argumentIt )
            {
                jsonArguments.append( *argumentIt );
            }

            jsonFlag[ "arguments" ] = jsonArguments;

            Json::Value jsonProperties = Json::Value( Json::arrayValue );

            std::vector< std::string >::iterator propertyIt = flag.properties.begin();
            std::vector< std::string >::iterator propertyEnd = flag.properties.end();

            for ( ; propertyIt != propertyEnd; ++propertyIt )
            {
                jsonProperties.append( *propertyIt );
            }

            jsonFlag[ "properties" ] = jsonProperties;

            jsonFlags.append( jsonFlag );
        }

        std::stringstream stream;
        stream << jsonRoot;

        MPxCommand::setResult( stream.str().c_str() );

        return true;
    }

    return false;
}

namespace {

std::string asString( MSyntax::MArgType type )
{
    switch( type )
    {
    case MSyntax::kBoolean: return "bool";
    case MSyntax::kLong: return "int";
    case MSyntax::kDouble: return "double";
    case MSyntax::kString: return "string";
    case MSyntax::kUnsigned: return "unsigned int";
    case MSyntax::kDistance: return "distance";
    case MSyntax::kAngle: return "angle";
    case MSyntax::kTime: return "time";
    case MSyntax::kSelectionItem: return "selection item";
    case MSyntax::kInvalidArgType: return "invalid argument type";
    case MSyntax::kNoArg: return "no argument type";
    case MSyntax::kLastArgType: return "last argument type";
    }

    return "arg type not found";
}

}

void CommandSpec::addFlag(
        const std::string& shortName,
        const std::string& longName,
        const std::string& category,
        commandproperties::PropertyBitMask properties,
        const std::string& description,
        MSyntax::MArgType argType1,
        MSyntax::MArgType argType2,
        MSyntax::MArgType argType3,
        MSyntax::MArgType argType4,
        MSyntax::MArgType argType5,
        MSyntax::MArgType argType6
        )
{
    if ( std::find( m_categories.begin(), m_categories.end(), category ) == m_categories.end() )
    {
        m_categories.push_back( category );
    }

    std::vector< std::string > arguments;
    MSyntax::MArgType types[6] = { argType1, argType2, argType3, argType4, argType5, argType6 };
    for ( size_t i=0; i<6; ++i )
    {
        if ( types[i] != MSyntax::kNoArg )
        {
            arguments.push_back( asString( types[i] ) );
        }
    }

    std::vector< std::string > propertyNames;
    if ( properties & commandproperties::kCreate )
    {
        propertyNames.push_back( "create" );
    }
    if ( properties & commandproperties::kQuery )
    {
        propertyNames.push_back( "query" );
    }
    if ( properties & commandproperties::kMultiUse )
    {
        propertyNames.push_back( "multi-use" );
    }


    m_flags.push_back( 
            Flag(
                shortName,
                longName,
                category,
                description,
                arguments,
                propertyNames
                )
            );

    // Register flags with actual MSyntax object.  Not quite sure why we don't need to prepend the
    // "-" at this point, but Maya doesn't seem to like it if we do
    MStatus status = m_syntax.addFlag(
            shortName.c_str(),
            longName.c_str(),
            argType1,
            argType2,
            argType3,
            argType4,
            argType5,
            argType6
            );

    if ( properties & commandproperties::kMultiUse )
    {
        m_syntax.makeFlagMultiUse( shortName.c_str() );
    }
    if ( properties & commandproperties::kQuery )
    {
        m_syntax.enableQuery();
    }
    if ( properties & commandproperties::kEdit )
    {
        m_syntax.enableEdit();
    }


    // Register flags with secondary MSyntax object that we use for -h query
    status = m_noArgsSyntax.addFlag(
            shortName.c_str(),
            longName.c_str()
            );
}

}

