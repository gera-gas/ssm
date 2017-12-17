##
## Create DOT file with implemented state machine for Graphviz (WebGraphviz).
## Graph can created by web tool: http://www.webgraphviz.com/
## ATENTION!: The source file should contain only one state machine implementation.
##
class StateMachineDescriptor
	attr_accessor :name, :first_state
	def initialize(name, first_state)
		@name = name
		@first_state = first_state
	end
end

class StateMachineEdge
	attr_accessor :from, :to, :comment
	def initialize(from, to, comment)
		@from = from
		@to = to
		@comment = comment
	end
end

$state_machine_list = []
$state_machine_edges = []

if ARGV.size == 0 then
	puts "ERROR: to few arguments for script."
	puts ""
	puts "    $ create_graph.rb state_machine_implementation.c"
	puts ""
	exit
end

if not File.exist?(ARGV[0]) then
	puts "ERROR: unable to open file '#{ARGV[0]}'"
	exit
end

File.open( ARGV[0], 'r' ).readlines.each do |line|
	if(line.include? "SM_CREATE(") then
		start_index = line.index("SM_CREATE(") + "SM_CREATE(".length
		len = line.index(")") - start_index
		line = line[start_index, len]
		line.delete!(' ')
		lines = line.split(',')
		#puts lines
		$state_machine_list << StateMachineDescriptor.new(lines.first, lines.last)
	end
end

processing_state = "wait_state"
open_braces_counter = 0
state_name = ''

def calc_opened_braces(counter, line)
	return counter = counter + line.count("{") - line.count("}")
end

File.open( ARGV[0], 'r' ).readlines.each do |line|
	continue if line.length == 0
	case processing_state
	when "wait_state"
		if line.include? "SM_STATE(" then
			processing_state = "scan_edges"
			open_braces_counter = calc_opened_braces(open_braces_counter, line)
			start_index = line.index("SM_STATE(") + "SM_STATE(".length
			len = line.index(")") - start_index
			state_name = line[start_index, len]
			state_name.delete!(' ')
		end

	when "scan_edges"
		open_braces_counter = calc_opened_braces(open_braces_counter, line)
		if open_braces_counter == 0 then
			processing_state = "wait_state"
		else
			jump_name = ''
			jump_name = "SM_RESET" if line.include? "SM_RESET"
			jump_name = "SM_JUMP(" if line.include? "SM_JUMP("
			if jump_name.length > 0 then
				from = state_name
				to = ''
				comment = ''
				if jump_name == "SM_JUMP(" then
					start_index = line.index(jump_name) + jump_name.length
					len = line.index(")") - start_index
					to = line[start_index, len]
					to.delete!(' ')
				else
					to = 'idle'
				end
				if line.include? "//" then
					comment = line[line.index("//") + "//".length, line.length]
					comment.strip!					
				elsif line.include? "/*" then
					start_index = line.index("/*") + "/*".length
					len = line.index("*/") - start_index
					comment = line[start_index, len]
					comment.strip!
				end
				$state_machine_edges << StateMachineEdge.new(from, to, comment)
			end
		end
	end
end

$state_machine_list.each do |e|
	puts "STATE-MACHINE:"
	puts "name: #{e.name}"
	puts "first_state: #{e.first_state}"
end
puts
puts
$state_machine_edges.each do |e|
	puts "STATE-MACHINE-EDGES:"
	puts "from: #{e.from}"
	puts "to: #{e.to}"
	puts "comment: #{e.comment}"
end

$state_machine_list.each do |e|
	f = File.new(e.name + ".dot", 'w')
	f.puts "digraph finite_state_machine {"
	f.puts "\trankdir=\"\";"
	f.puts "\tsize=\"10\""
	f.puts "\tnode [shape = doublecircle]; idle;"
	f.puts "\tnode [shape = rectangle];"
	f.puts "\tidle -> #{e.first_state};"
	$state_machine_edges.each do |edge|
		f.puts "\t#{edge.from} -> #{edge.to} [ label = \"#{edge.comment}\" ];"
	end
	f.puts "}"
	f.close
end
