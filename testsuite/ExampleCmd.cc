
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
    using namespace ma::commandproperties;

    const char* kGeneral = "General";

    s_commandSpec.addFlag(
            "l",
            "long",
            kGeneral,
            kCreate,
            "This is an int flag, I'd call it int but Maya doesn't like flag long names which are "
            "less than 4 characters.",
            MSyntax::kLong
            );

    s_commandSpec.addFlag(
            "d",
            "double",
            kGeneral,
            kCreate | kQuery,
            "This is a double flag",
            MSyntax::kDouble
            );

    s_commandSpec.addFlag(
            "s",
            "string",
            kGeneral,
            kCreate | kQuery | kMultiUse,
            "This is a string flag",
            MSyntax::kString
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

