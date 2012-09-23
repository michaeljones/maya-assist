
#include "CommandFlags.hh"

#include <maya/MPxCommand.h>
#include <maya/MArgParser.h>

namespace ma {
    
CommandFlags::CommandFlags( const char* name, const char* description )
 : m_name( name ),
   m_description( description ),
   m_syntax()
{
    const char* kHelp = "Help";
    addFlag(
            "-h",
            "-help",
            kHelp,
            "Returns the help"
           );

    addFlag(
            "-hj",
            "-helpJson",
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
        MPxCommand::setResult( "This is the help string" );
        return true;
    }
    else if ( argParser.isFlagSet( "-hj" ) || argParser.isFlagSet( "-helpJson" ) )
    {
        MPxCommand::setResult( "This is the json help string" );
        return true;
    }

    return false;
}

void CommandFlags::addFlag(
        const char* shortName,
        const char* longName,
        const char* category,
        const char* description
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
            shortName,
            longName
            );

    status = m_noArgsSyntax.addFlag(
            shortName,
            longName
            );
}


}

