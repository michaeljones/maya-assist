
from docutils.parsers.rst.directives.tables import Table
from docutils.parsers import rst
from docutils import nodes

from ma.command import MayaSession

import json


class RequiresMaya(rst.Directive):

    def __init__(self, maya_session, *args):

        rst.Directive.__init__(self, *args)

        self.maya_session = maya_session

class CommandFlags(RequiresMaya):

    required_arguments = 1
    option_spec = {}
    has_content = False

    def run(self):

        command_name = self.arguments[0]

        self.maya_session.loadPlugin("examplePlugin")
        command = getattr(self.maya_session, command_name)

        json_string = command(helpJson=True)

        data = json.loads(json_string)

        table_node = self.build_table_from_json(data)

        table_node['classes'] += self.options.get('class', [])

        messages = []
        return [table_node] + messages

    def build_table_from_json(self, json):

        table = nodes.table()

        col_widths = [50, 50, 50]
        tgroup = nodes.tgroup(cols=len(col_widths))
        table += tgroup

        for col_width in col_widths:
            colspec = nodes.colspec(colwidth=col_width)
            tgroup += colspec

        rows = []
        header_row = nodes.row()

        name_entry = nodes.entry()
        name_entry += nodes.paragraph( "", "", nodes.Text( "Long name (short name)" ) )
        header_row += name_entry

        argument_entry = nodes.entry()
        argument_entry += nodes.paragraph( "", "", nodes.Text( "Argument Types" ) )
        header_row += argument_entry

        mode_entry = nodes.entry()
        mode_entry += nodes.paragraph( "", "", nodes.Text( "Properties" ) )
        header_row += mode_entry

        rows.append(header_row)

        for flag in json["flags"]:

            # Details row
            row_node = nodes.row()

            # Name entry
            entry = nodes.entry()
            text = "%s (%s)" % ( flag["longname"], flag["shortname"] )
            entry += nodes.paragraph( "", "", nodes.Text( text ) )
            row_node += entry

            # Arguments entry
            entry = nodes.entry()
            text = " ".join( flag["arguments"] )
            entry += nodes.paragraph( "", "", nodes.Text( text ) )
            row_node += entry

            # Properties entry
            entry = nodes.entry()
            text = " ".join( flag["properties"] )
            entry += nodes.paragraph( "", "", nodes.Text( text ) )
            row_node += entry

            rows.append(row_node)

            # Description row
            row_node = nodes.row()

            # Description
            entry = nodes.entry()
            text = flag["description"]
            entry += nodes.paragraph( "", "", nodes.Text( text ) )
            entry["morecols"] = 2
            row_node += entry

            rows.append(row_node)

        header_rows = False
        # if header_rows:
        #     thead = nodes.thead()
        #     thead.extend(rows[:header_rows])
        #     tgroup += thead
        tbody = nodes.tbody()
        tbody.extend(rows[header_rows:])
        tgroup += tbody
        return table

class DirectiveContainer(object):

    def __init__(
            self,
            directive,
            maya_session_container
            ):

        self.directive = directive
        self.maya_session_container = maya_session_container

        # Required for sphinx to inspect
        self.required_arguments = directive.required_arguments
        self.optional_arguments = directive.optional_arguments
        self.option_spec = directive.option_spec
        self.has_content = directive.has_content

    def __call__(self, *args):

        call_args = []
        call_args.append(self.maya_session_container.get_maya_session())
        call_args.extend(args)

        return self.directive(*call_args)


class MayaSessionContainer(object):

    def __init__(self):
        self.maya_session = None

    def get_maya_session(self):
        return self.maya_session

    def setup(self, app):

        self.maya_session = MayaSession(app.config.ma_mayapy_path)

    def close(self, app, exception):

        self.maya_session.close()

def setup(app):

    session_container = MayaSessionContainer()

    app.add_directive(
            'ma-command-flags',
            DirectiveContainer(CommandFlags, session_container)
            )
    
    app.add_config_value("ma_mayapy_path", "", "env")

    app.connect("builder-inited", session_container.setup)
    app.connect("build-finished", session_container.close)


