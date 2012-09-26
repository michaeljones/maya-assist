
#include "CommandFlags.hh"

#include <maya/MPxCommand.h>
#include <maya/MArgParser.h>

#include <json/json.h>

#include <sstream>
#include <list>

namespace ma {
    
CommandFlags::CommandFlags( const char* name, const char* arguments, const char* description )
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


CommandFlags::operator MSyntax()
{
    return m_syntax;
}

bool CommandFlags::handle( const MArgList& args )
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

        std::list< std::string > categories;

        for ( size_t i=0; i<m_flags.size(); ++i )
        {
            if ( std::find( categories.begin(), categories.end(), m_flags[i].category ) == categories.end() )
            {
                categories.push_back( m_flags[ i ].category );
            }
        }

        std::list< std::string >::iterator categoryIt = categories.begin();
        std::list< std::string >::iterator categoryEnd = categories.end();

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
        Json::Value root;
        root[ "name" ] = m_name;
        root[ "description" ] = m_description;

        Json::Value& flags = root[ "flags" ];

        std::list< std::string > categories;

        for ( size_t i=0; i<m_flags.size(); ++i )
        {
            if ( std::find( categories.begin(), categories.end(), m_flags[i].category ) == categories.end() )
            {
                categories.push_back( m_flags[ i ].category );
            }
        }

        std::list< std::string >::iterator categoryIt = categories.begin();
        std::list< std::string >::iterator categoryEnd = categories.end();

        for ( ; categoryIt != categoryEnd; ++categoryIt )
        {
            Json::Value& category = flags[ *categoryIt ];

            for ( size_t i=0; i<m_flags.size(); ++i )
            {
                Flag& flag = m_flags[ i ];
                if ( flag.category == *categoryIt )
                {
                    Json::Value jsonFlag;

                    jsonFlag[ "shortname" ] = flag.shortName;
                    jsonFlag[ "longname" ] = flag.longName;
                    jsonFlag[ "description" ] = flag.description;

                    category.append( jsonFlag );
                }
            }
        }

        std::stringstream stream;
        stream << root;

        MPxCommand::setResult( stream.str().c_str() );

        return true;
    }

    return false;
}

void CommandFlags::addFlag(
        const std::string& shortName,
        const std::string& longName,
        const std::string& category,
        const std::string& description
        )
{
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

