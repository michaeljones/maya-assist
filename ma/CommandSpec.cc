
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
            "Shows the help"
           );

    addFlag(
            "hj",
            "helpJson",
            kHelp,
            "Returns the help in a json structure"
           );
}


CommandSpec::operator MSyntax()
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

            jsonFlags.append( jsonFlag );
        }

        std::stringstream stream;
        stream << jsonRoot;

        MPxCommand::setResult( stream.str().c_str() );

        return true;
    }

    return false;
}

void CommandSpec::addFlag(
        const std::string& shortName,
        const std::string& longName,
        const std::string& category,
        const std::string& description
        )
{
    if ( std::find( m_categories.begin(), m_categories.end(), category ) == m_categories.end() )
    {
        m_categories.push_back( category );
    }

    m_flags.push_back( 
            Flag(
                shortName,
                longName,
                category,
                description
                )
            );

    MStatus status = m_syntax.addFlag(
            std::string( "-" + shortName ).c_str(),
            std::string( "-" + longName ).c_str()
            );

    status = m_noArgsSyntax.addFlag(
            shortName.c_str(),
            longName.c_str()
            );
}

}

