
#include "ExampleCmd.hh"

#include <maya/MArgDatabase.h>
#include <maya/MArgList.h>

ma::CommandFlags ExampleCmd::s_commandFlags(
        "exampleCommand",
        "This is a long description of what this command does. It can"
        "span multiple lines if desired."
        );

void* ExampleCmd::creator()
{
    return new ExampleCmd;
}

MSyntax ExampleCmd::syntaxCreator()
{
    const char* kGeneral = "General";

    s_commandFlags.addFlag(
            "-l",
            "-long",
            kGeneral,
            "description"
            );

    s_commandFlags.setObjectType( MSyntax::kStringObjects, 1, 2 );

    return s_commandFlags;
}

MStatus ExampleCmd::doIt( const MArgList& args )
{
    if ( s_commandFlags.handle( args ) )
    {
        return MS::kSuccess;
    }

    return MS::kSuccess;
};

