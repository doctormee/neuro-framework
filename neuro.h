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
    static double computing_function( List <Edge>  ); //computing function, is static for all nodes and (in our scenario) is a weighted sum
    // enum n_type{ input, hidden, output, bias }
public:
    List <Edge> input_list, output_list; //these are two Edge lists storing all edges going in and out of the node
    Node(){ activation_value = 0; activation_function = sgn;} //simple constructor, used for making a Bias
    Node( double ( *func )( double ) );
    void compute() const { y = activation_function( computing_function( input_list ) ); } //calculates and sets a proper output value 
    void set_activation_value( double x ) const { activation_value = x; } // setter
    double get_activation_value() const { return activation_value; } // getter
};
/* this is a main Net class, which is a neuro-net 
it should provide proper interface
*/ 


class Net{
    List <Node> input_layer, output_layer;
    List <List <Node> > hidden_layers;
public:
    Net( int hidden_layer_count, int input_node_count, int hidden_node_count, int output_node_count, 
    double ( *hidden_activation_function)( double ), double ( *output_activation_function)( double ) ); //net constructior
    ~Net();
    List <double> activate( List <double> input ); //attnt: input is set by val, not by ref
    void back_propagate( List <double> output, List <double> ideal );
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
    List** temp;
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
        val += inp.*elem.*from.get_y() * inp.*elem.weight; 
        inp = inp.*next;
    }
    return val;
}
Node::Node( double ( *func )( double ) ){
    activation_value = 0;
    activation_function = func;
}
/* this section is solely for defining Net methods */
Net::Net( int hidden_layer_count, int input_node_count, int hidden_node_count, int output_node_count, 
double ( *hidden_activation_function)( double ), double ( *output_activation_function)( double ) ){
    srand( time( nullptr ) );
    //initializing input_layer with new Nodes
    while ( input_node_count-- ){
        input_layer.append( new Node );
    }
    int nodes_left;
    List <Node> *hidden_layer; //new hidden layer, later set to be appended at the end of hidden_layers
    List <Node> *link_layer = &input_layer;  //previous layer
    List <Node> *current_link_layer; //current position at lik_layer for each new node
    Node *new_node; //our new node
    Edge *e; //a dummy edge ptr to create edges
    while ( hidden_layer_count-- ){     //initializing all hidden layers    
        hidden_layer = new List;
        nodes_left = hidden_node_count;
        while ( nodes_left-- ){        //initializing a new hidden layer
            current_link_layer = link_layer;
            new_node = new Node( hidden_activation_function );
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
        new_node = new Node( output_activation_function );
        output_layer.append( new_node );
        while ( current_link_layer != nullptr ){
            e = new Edge( *current_link_layer.elem, new_node, rand() % 200 - 100 ); //initializes a random-weighted ( -100 -- 100 ) edge between link_node and new_node
            current_link_layer = *current_link_layer.next;
        }
    }
}
    