## state_machine 
Standalone low-memory state machine implementation. This is small state machine developed for using in any software project types C/C++ (embedded, Windows, etc) and low memory MCU for multitask processing imitation. Ruby script for automatically generate graph on C state machine sources.

## how to use
Simply paste sources of state machine implementation from `src` directory into your project.
Details example of using can see [here](demo/sm_demo.c).

## create (draw) graph
For graph creation you should ruby script from `script/create_graph.rb` with path to source file with state machine implementation.
    
    $ ruby create_graph.rb source_file_with_state_machine.c 

Graph can created from `*.dot` file by web tool [WebGraphviz](http://www.webgraphviz.com/)
