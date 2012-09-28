
#include "ExampleCmd.hh"

#include <maya/MArgDatabase.h>
#include <maya/MArgList.h>

ma::CommandSpec ExampleCmd::s_commandSpec(
        "exampleCommand",
        "",
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

    s_commandSpec.addFlag(
            "l",
            "long",
            kGeneral,
            "description"
            );

    s_commandSpec.setObjectType( MSyntax::kStringObjects, 1, 2 );

    return s_commandSpec.syntax();
}

MStatus ExampleCmd::doIt( const MArgList& args )
{
    if ( s_commandSpec.handle( args ) )
    {
        return MS::kSuccess;
    }

    return MS::kSuccess;
};

