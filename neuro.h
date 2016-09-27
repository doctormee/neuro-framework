#include <new>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
double sgn(double val) {
    return double ( ( 0 < val ) - ( val < 0 ) );
}
/* this is an Edge struct (which is equal to a public class)
It stores index and destination Node pointers and a weight of an edge
*/
struct Edge{
    Node *from, *to;
    double weight;
    Edge( Node, Node, double );
};

/* this is a list*/
template <class T>
struct List{
    T *elem;
    List *next;
    List();
    List( T );
    void append( T ); // appends *piece at the end of a list 
};
/* this is a main class, implementing a node structure with all necessary 
values and methods
*/
class Node{
    double activation_value; //output value
    double ( *activation_function )( double ); //pointer to the activation function
    double ( *activation_function_derivative )( double ); //pointer to the activation function's first derivative
    static double computing_function( List <Edge>  ); //computing function, is static for all nodes and (in our scenario) is a weighted sum
    // enum n_type{ input, hidden, output, bias }
public:
    List <Edge> input_list, output_list; //these are two Edge lists storing all edges going in and out of the node
    Node(){ activation_value = 0; activation_function = sgn; activation_function_derivative = 0;} //simple constructor, used for making a Bias
    Node( double ( *func )( double ), double ( *deriv )( double ) );
    void activate() const { y = activation_function( computing_function( input_list ) ); } //calculates and sets a proper output value 
    void set_activation_value( double x ) const { activation_value = x; } // setter
    double get_activation_value() const { return activation_value; } // getter
};
/* this is a main Net class, which is a neuro-net 
it should provide proper interface
*/ 


class Net{
    List <Node> input_layer, output_layer;
    List <List <Node> > hidden_layers;
    double learning_rate;
public:
    Net( int hidden_layer_count, int input_node_count, int hidden_node_count, int output_node_count, 
    double ( *hidden_activation_function)( double ), double ( *output_activation_function)( double ) ); //net constructior
    ~Net();
    List <double> activate( List <double> input ); //attnt: input is sent by val, not by ref
    void back_propagate( List <double> output, List <double> ideal, double learning_rate );
};
//
//
//
//
//
/* this section is solely for defining Edge methods */
Edge::Edge( Node *f, Node *t, double w ){
    to = t;
    from = f;
    weight = w;
    *f.output_list.append( this );
    *t.input_list.append( this );
}
/* this section is solely for defining List methods */
template <class T>
List<T>::List(){
    next = nullptr;
    elem = nullptr;
}
template <class T>
List<T>::List( T *init ){
    next = nullptr;
    elem = init; 
}
template <class T>
void List<T>::append( T *piece ) {
    List<T>** temp;
    *temp = next;
    while ( *temp != nullptr ){
        *temp = *next.next;
    }
    **temp.next = nullptr;
    **temp.elem = piece;
}
/* this section is solely for defining Node methods */
double Node::computing_function( List <Edge> inp ){
    double val = 0;
    while ( inp.next != nullptr ){
        val += inp.*elem.*from.get_activation_value() * inp.*elem.weight; 
        inp = inp.*next;
    }
    val += inp.*elem.*from.get_activation_value() * inp.*elem.weight;
    return val;
}
Node::Node( double ( *func )( double ), double ( *deriv )( double ) ){
    activation_value = 0;
    activation_function = func;
    activation_function_derivative = deriv;
}
/* this section is solely for defining Net methods */
Net::Net( int hidden_layer_count, int input_node_count, int hidden_node_count, int output_node_count, 
double ( *hidden_activation_function)( double ), double ( *hidden_activation_function_derivative )( double ), double ( *output_activation_function)( double ), double ( *output_activation_function_derivative )( double ) ){
    srand( time( nullptr ) );
    //initializing input_layer with new Nodes
    while ( input_node_count-- ){
        input_layer.append( new Node );
    }
    int nodes_left;
    List <Node> *hidden_layer; //new hidden layer, later set to be appended at the end of hidden_layers
    List <Node> *link_layer = &input_layer;  //previous layer
    List <Node> *current_link_layer; //current position at link_layer for each new node
    Node *new_node; //our new node
    Edge *e; //a dummy edge ptr to create edges
    while ( hidden_layer_count-- ){     //initializing all hidden layers    
        hidden_layer = new List;
        nodes_left = hidden_node_count;
        while ( nodes_left-- ){        //initializing a new hidden layer
            current_link_layer = link_layer;
            new_node = new Node( hidden_activation_function, hidden_activation_function_derivative );
            *hidden_layer.append( new_node );
            while ( current_link_layer != nullptr ){
                e = new Edge( *current_link_layer.elem, new_node, rand() % 200 - 100 ); //initializes a random-weighted ( -100 -- 100 ) edge between link_node and new_node
                current_link_layer = *current_link_layer.next;
                }
        }
        //and finally append our new initialized and linked layer to the hidden_layers
        hidden_layers.append( *hidden_layer );
        //finding the next link_layer
        link_layer = hidden_layer; 
    }
    while ( output_node_count-- ){
        current_link_layer = link_layer;
        new_node = new Node( output_activation_function, output_activation_function_derivative );
        output_layer.append( new_node );
        while ( current_link_layer != nullptr ){
            e = new Edge( *current_link_layer.elem, new_node, rand() % 200 - 100 ); //initializes a random-weighted ( -100 -- 100 ) edge between link_node and new_node
            current_link_layer = *current_link_layer.next;
        }
    }
}
Net::~Net(){
    List <Node> *current_layer_ptr = &input_layer;
    List <List <Node> > *hidden_layers_ptr = &hidden_layers;
    List <Edge> *edge_ptr;
    while ( current_layer_ptr != nullptr ){
        edge_ptr = *current_layer_ptr.*elem.&output_list;
        while ( edge_ptr != nullptr ){
            delete *edge_ptr.elem;
            edge_ptr = *edge_ptr.next;
        }
        delete *current_layer_ptr.elem;
        current_layer_ptr = *current_layer_ptr.next;
    }
    while ( hidden_layers_ptr != nullptr ){
        current_layer_ptr = *hidden_layers_ptr.elem;
        while ( current_layer_ptr != nullptr ){
            edge_ptr = *current_layer_ptr.*elem.&output_list;
            while ( edge_ptr != nullptr ){
                delete *edge_ptr.elem;
                edge_ptr = *edge_ptr.next;
            }
            delete *current_layer_ptr.elem;
            current_layer_ptr = *current_layer_ptr.next;
        }
        delete *hidden_layers_ptr.elem;
        hidden_layers_ptr = *hidden_layers_ptr.next;
    }
    current_layer_ptr = &output_layer;
    while ( current_layer_ptr != nullptr ){
        delete *current_layer_ptr.elem;
        current_layer_ptr = *current_layer_ptr.next;
    }
}
List <double> Net::activate( List <double> input ){
    List <double> ans;
    List <Node> *current_layer_ptr = &input_layer;
    List <List <Node> > *hidden_layers_ptr = &hidden_layers;
    while ( ( input.next != nullptr ) && ( current_layer_ptr != nullptr ) ){
        *current_layer_ptr.*elem.set_activation_value( input.*elem );
        input = input.*next;
        current_layer_ptr = *current_layer_ptr.next;
    }
    if ( ( current_layer_ptr != nullptr ) && ( input.next == nullptr  ) ){
        *current_layer_ptr.*elem.set_activation_value( input.*elem );
    }
    while ( hidden_layers_ptr != nullptr ){
        current_layer_ptr = *hidden_layers_ptr.elem;
        while ( current_layer_ptr != nullptr ){
            *current_layer_ptr.*elem.activate();
            current_layer_ptr = *current_layer_ptr.next;
        }
        hidden_layers_ptr = *hidden_layers_ptr.next;
    }
    current_layer_ptr = &output_layer;
    while ( current_layer_ptr != nullptr ){
        *current_layer_ptr.*elem.activate();
        ans.append( *current_layer_ptr.*elem.get_activation_value() );
        current_layer_ptr = *current_layer_ptr.next;
    }
    return ans;
}
    