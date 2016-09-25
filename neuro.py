class Edge: #Edge( from_node, weight, to_node )
    def __init__(self, from_node=None, weight=None, to_node=None):
        self._from_node = from_node
        self._to_node = to_node
        self._weight = weight

    def assign_as_input_to_node(self, node):
        node.add_input_edge(self)
    def assign_as_output_to_node(self, node):
        node.add_output_edge(self)
    def assign_to_nodes(self):
        self.assign_as_output_to_node(self._from_node)
        self.assign_as_input_to_node(self._to_node)

    @property
    def from_node(self):
        return self._from_node
    @from_node.setter
    def from_node(self, node):
        self._from_node = node

    @property
    def to_node(self):
        return self._to_node
    @to_node.setter
    def to_node(self, node):
        self._to_node = node

    @property
    def weight(self):
        return self._weight
    @weight.setter
    def weight(self, weight):
        self._weight = weight

NODE_TYPE_INPUT = "INPUT"
NODE_TYPE_HIDDEN = "HIDDEN"
NODE_TYPE_OUTPUT = "OUTPUT"
class Node:
    def __init__(self, activation_function, node_type):
        self._activation_function = activation_function
        self._node_type = node_type

        self._activation_value = 0
        self._input_edge_list = []
        self._output_edge_list = []

    @property
    def activation_value(self):
        return self._activation_value
    @activation_value.setter
    def activation_value(self, value):
        self._activation_value = value

    def activate(self):
        # if self._node_type == NODE_TYPE_INPUT:
        #     return
        self._activation_value = \
         self._activation_function(self._get_weighted_inputs())

    def _get_weighted_inputs(self):
        sum = 0
        for edge in self._input_edge_list:
            if edge.from_node is not None:
                sum += edge.weight * edge.from_node.activation_value
        return sum

    def add_input_edge(self, edge):
        if edge not in self._input_edge_list:
            self._input_edge_list.append(edge)
    def add_output_edge(self, edge):
        if edge not in self._output_edge_list:
            self._output_edge_list.append(edge)
            
    def dump(self):
        print("node dump")
        print("type: {}".format(self._node_type))
        print("activation_value: {}".format(self._activation_value))
        print("input list: {}".format(self._input_edge_list))
        print("output list: {}".format(self._output_edge_list))

class Layer:
    def __init__(self, name, node_type, node_count, node_activation_function):
        self.name = name
        self.node_type = node_type
        self.node_count = node_count

        self._node_list = []

        for i in range(node_count):
            self._node_list.append(Node(node_activation_function, node_type))

    @property
    def node_list(self):
        return self._node_list

    def activate(self):
        for node in self._node_list:
            node.activate()
class Neural_net:
    def __init__(self, hidden_layer_count, input_node_count, hidden_node_count,\
     output_node_count, hidden_activation_function, output_activation_function):
        #
        #   initialize layers
        #
        self._input_layer = Layer("input", "input", input_node_count, None)
        self._hidden_layer_list = []
        for i in range(hidden_layer_count):
            self._hidden_layer_list.append( Layer("hidden{}".format(i), "hidden", hidden_node_count, hidden_activation_function) )
        self._output_layer = Layer("output", "output", output_node_count, output_activation_function)

        #
        #   connect layers with randomized weights
        #
        def connect_layers( from_layer, to_layer ):
            for from_node in from_layer.node_list:
                for to_node in to_layer.node_list:
                    from random import uniform
                    rand = uniform(0.5, 2)
                    print(rand)
                    edge = Edge(from_node, rand, to_node)
                    edge.assign_to_nodes()
        connect_layers( self._input_layer, self._hidden_layer_list[0] )
        if hidden_layer_count > 1:
            for i in range(hidden_layer_count - 1):
                connect_layers( self._hidden_layer_list[i], self._hidden_layer_list[i+1] )
        connect_layers( self._hidden_layer_list[-1], self._output_layer )

    def activate(self, *input_vector):
        if len(input_vector) != len(self._input_layer.node_list):
            raise "input vector length not eual to input layer length"

        for i in range(len(input_vector)):
            self._input_layer.node_list[i].activation_value = input_vector[i]

        for layer in self._hidden_layer_list:
            layer.activate()

        self._output_layer.activate()

        result = []
        for node in self._output_layer.node_list:
            result.append( node.activation_value )

        return result

    def dump_nodes(self):
        for node in self._input_layer.node_list:
            node.dump()
        for node in self._hidden_layer_list[0].node_list:
            node.dump()
        for node in self._output_layer.node_list:
            node.dump()


def ret_1(x):
    return 1
def act_func(x):
    from math import exp
    return 1/(1 + exp(-x))


inp1 = Node(ret_1, "input")
inp2 = Node(ret_1, "input")

hidden = Node(act_func, "hidden")

edge1 = Edge(inp1,1,hidden)
inp1.add_output_edge(edge1)
hidden.add_input_edge(edge1)

edge2 = Edge(from_node = inp2, weight = 1, to_node = hidden)
inp2.add_output_edge(edge2)
hidden.add_input_edge(edge2)


inp1.activate()
inp2.activate()
hidden.activate()


net = Neural_net(1, 2, 1, 1, act_func, act_func)

print( net.activate(1,1) )
net.dump_nodes()

# def __init__(self, hidden_layer_count, input_node_count, hidden_node_count,\
#  output_node_count, hidden_activation_function, output_activation_function):

#print(hidden.activation_value)



