class Edge:
    def __init__(self, from_node=None, weight=None, to_node=None):
        self._from_node = from_node
        self._to_node = to_node
        self._weight = weight

        self._old_delta = 0


    #
    #   assigning edge to nodes
    #
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

    @property
    def old_delta(self):
        return self._old_delta
    @old_delta.setter
    def old_delta(self, delta):
        self._old_delta = delta

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
    #
    #   edge lists
    #
    @property
    def input_edge_list(self):
        return self._input_edge_list
    @property
    def output_edge_list(self):
        return self._output_edge_list

    def add_input_edge(self, edge):
        if edge not in self._input_edge_list:
            self._input_edge_list.append(edge)
    def add_output_edge(self, edge):
        if edge not in self._output_edge_list:
            self._output_edge_list.append(edge)
    #
    #   activation
    #
    @property
    def activation_value(self):
        return self._activation_value
    @activation_value.setter
    def activation_value(self, value):
        self._activation_value = value

    def activate(self):
        self._activation_value = \
         self._activation_function(self._get_weighted_inputs())

    def _get_weighted_inputs(self):
        sum = 0
        for edge in self._input_edge_list:
            if edge.from_node is not None:
                sum += edge.weight * edge.from_node.activation_value
        return sum
    #
    #   Error
    #
    @property
    def error(self):
        return self._error
    @error.setter
    def error(self, error):
        self._error = error

    def calculate_error(self):
        error = 0.0
        for edge in self._output_edge_list:
            error += edge.to_node.error * edge.weight
        self._error = error * self._activation_value * (1 - self._activation_value)

         
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

    def calculate_error(self):
        for node in self._node_list:
            node.calculate_error()

    def update_weights_of_input_edges(self, learning_rate, momentum):
        for node in self._node_list:
            for edge in node.input_edge_list:
                delta = learning_rate * node.error * edge.from_node.activation_value + momentum * edge.old_delta
                edge.weight += delta
                edge.old_delta = delta

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
                    from random import random
                    rand = random()*4 - 2
                    #print(rand)
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

    def back_propagate(self, input_vector, desired_output_vector, learning_rate, momentum, calculate_average_error = False):
        result = self.activate(*input_vector)
        average_error = 0
        for i in range(len(self._output_layer.node_list)):
            if calculate_average_error:
                average_error +=  (desired_output_vector[i] - result[i])**2
            node = self._output_layer.node_list[i] 
            node.error = (desired_output_vector[i] - result[i]) * node.activation_value*(1 - node.activation_value)
        self._output_layer.update_weights_of_input_edges(learning_rate, momentum)

        for layer in self._hidden_layer_list[::-1]:
            layer.calculate_error()
            layer.update_weights_of_input_edges(learning_rate, momentum)

        return average_error / len(result)



    def dump_nodes(self):
        for node in self._input_layer.node_list:
            node.dump()
        for layer in self._hidden_layer_list:
            for node in layer.node_list:
                node.dump()
        for node in self._output_layer.node_list:
            node.dump()


def ret_1(x):
    return 1
def act_func(x):
    from math import exp
    return 1.0/(1.0 + exp(-x))


# inp1 = Node(ret_1, "input")
# inp2 = Node(ret_1, "input")

# hidden = Node(act_func, "hidden")

# edge1 = Edge(inp1,1,hidden)
# inp1.add_output_edge(edge1)
# hidden.add_input_edge(edge1)

# edge2 = Edge(from_node = inp2, weight = 1, to_node = hidden)
# inp2.add_output_edge(edge2)
# hidden.add_input_edge(edge2)


# inp1.activate()
# inp2.activate()
# hidden.activate()

# def __init__(self, hidden_layer_count, input_node_count, hidden_node_count,\
#  output_node_count, hidden_activation_function, output_activation_function):
net = Neural_net(1, 2, 40, 1, act_func, act_func)

print("network generated, starting iteration")

print(net.activate(0,0))
print(net.activate(0,1))
print(net.activate(1,0))
print(net.activate(1,1))
print()

ler_rate = 1
momentum = 0.1
for i in range(1000):
    net.back_propagate([0,0], [0], ler_rate, momentum)
    net.back_propagate([0,1], [1], ler_rate, momentum)
    net.back_propagate([1,0], [1], ler_rate, momentum)
    if i %50 == 0:
        print(net.back_propagate([1,1], [0], ler_rate, momentum, calculate_average_error = True))
    else:
        net.back_propagate([1,1], [0], ler_rate, momentum)

print(net.activate(0,0))
print(net.activate(0,1))
print(net.activate(1,0))
print(net.activate(1,1))

# from random import random

# for i in range(1):
#     list = []
#     for _ in range(120):
#         list.append(random())
#     print(net.activate(*list))

#print(net.dump_nodes())



#print(hidden.activation_value)


