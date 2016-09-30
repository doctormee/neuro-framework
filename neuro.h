#include <new>
#include <stdlib.h>     /* srand, rand */
#include <time.h> 
#include <iostream>      /* time */
double sgn(double val) {
    return double ( ( 0 < val ) - ( val < 0 ) );
}
double inp_func( double val ){
    return val;
}
double inp_deriv( double val ){
    return 1;
}
class Node;
/* this is an Edge struct (which is equal to a public class)
It stores index and destination Node pointers and a weight of an edge
*/
struct Edge{
    Node *from, *to;
    double weight, prev_weight;
    Edge( Node*, Node*, double );
};

/* this is a list*/
template <class T> 
struct List{
    T *elem;
    List *next;
    int length;
    List();
    ~List();
    List( T* );
    void append( T* ); // appends *piece at the end of a list 
};
struct Dlist{
    double elem;
    Dlist *next;
    int length;
    Dlist( double init ){
        next = nullptr;
        elem = init;
        length = 1;
    }
    Dlist(){
        next = nullptr;
        length = 0;
    }
    void append( double piece ){
        Dlist* temp = next;
        if ( length == 0 ){
            elem = piece;
        }
        else{
            while ( temp != nullptr ){
                temp = temp->next;
            }
            temp = new Dlist( piece );
        }
        ++length;
    }
    ~Dlist(){
        if ( length > 1 )
            delete this->next;  
    }
};
/* this is a main class, implementing a node structure with all necessary 
values and methods
*/
class Node{
    double activation_value; //output value
    double ( *activation_function )( double ); //pointer to the activation function
    double ( *activation_function_derivative )( double ); //pointer to the activation function's first derivative
    double computing_function( List <Edge>  ); //computing function in our scenario is a weighted sum
    // enum n_type{ input, hidden, output, bias }
public:
    List <Edge> input_list, output_list; //these are two Edge lists storing all edges going in and out of the node
    double delta;
    Node(){ activation_value = 0; activation_function = sgn; activation_function_derivative = 0; delta = 0;} //simple constructor, used for making a Bias
    Node( double ( *func )( double ), double ( *deriv )( double ) );
    void activate() { activation_value = activation_function( computing_function( input_list ) ); } //calculates and sets a proper output value 
    double get_derivative_value() { return activation_function_derivative( computing_function( input_list ) ); }
    void set_activation_value( double x ) { activation_value = x; } // setter
    double get_activation_value() { return activation_value; } // getter
};
/* this is a main Net class, which is a neuro-net 
it should provide proper interface
*/ 


class Net{
    List <Node> input_layer, output_layer;
    List <List <Node> > hidden_layers;
public:
    Net( int hidden_layer_count, int input_node_count, int hidden_node_count, int output_node_count, 
    double ( *hidden_activation_function)( double ), double ( *hidden_activation_function_derivative )( double ), double ( *output_activation_function)( double ), double ( *output_activation_function_derivative )( double ) ); //net constructior
    ~Net();
    Dlist activate( Dlist input ); //attnt: input is sent by val, not by ref
    void back_propagate( Dlist input, Dlist ideal, double learning_rate, double momentum );
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
    if ( f != nullptr )
        f->output_list.append( this );
    if ( t != nullptr )
        t->input_list.append( this );
}
/* this section is solely for defining List methods */
template <class T>
List<T>::List(){
    next = nullptr;
    elem = nullptr;
    length = 0;
}
template <class T>
List<T>::List( T *init ){
    next = nullptr;
    elem = init; 
    length = 1;
}
template <class T>
void List<T>::append( T *piece ) {
    List<T>* temp = next;
    if ( length == 0 ){
        elem = piece;
    }
    else{
        while ( temp != nullptr ){
            temp = temp->next;
        }
        temp = new List( piece );
    }
    ++length;
}
template <class T>
List<T>::~List(){
    if ( length > 1 )
        delete this->next;
}
/* this section is solely for defining Node methods */
double Node::computing_function( List <Edge> inp ){
    double val = 0;
    while ( ( inp.next != nullptr ) && ( inp.elem != nullptr ) ){
        val = val + ( inp.elem->from->get_activation_value() * inp.elem->weight ); 
        inp = *( inp.next );
    }
    if ( inp.elem != nullptr )
        val += inp.elem->from->get_activation_value() * inp.elem->weight;
    //std::cout << val << '\n';
    return val;
}
Node::Node( double ( *func )( double ), double ( *deriv )( double ) ){
    activation_value = 0;
    activation_function = func;
    activation_function_derivative = deriv;
    delta = 0;
}
/* this section is solely for defining Net methods */
Net::Net( int hidden_layer_count, int input_node_count, int hidden_node_count, int output_node_count, 
double ( *hidden_activation_function)( double ), double ( *hidden_activation_function_derivative )( double ), double ( *output_activation_function)( double ), double ( *output_activation_function_derivative )( double ) ){
    srand( time( nullptr ) );
    //initializing input_layer with new Nodes
    while ( input_node_count-- ){
        input_layer.append( new Node( inp_func, inp_deriv ) );
    }
    int nodes_left;
    List <Node> *hidden_layer; //new hidden layer, later set to be appended at the end of hidden_layers
    List <Node> *link_layer = &input_layer;  //previous layer
    List <Node> *current_link_layer; //current position at link_layer for each new node
    Node *new_node; //our new node
    Edge *e; //a dummy edge ptr to create edges
    while ( hidden_layer_count > 0 ) {     //initializing all hidden layers    
        hidden_layer = new List <Node>;
        nodes_left = hidden_node_count;
        while ( nodes_left > 0 ){        //initializing a new hidden layer
            current_link_layer = link_layer;
            new_node = new Node( hidden_activation_function, hidden_activation_function_derivative );
            hidden_layer->append( new_node );
            while ( current_link_layer != nullptr ){
                e = new Edge( current_link_layer->elem, new_node, rand() % 100 ); //initializes a random-weighted edge between link_node and new_node
                current_link_layer = current_link_layer->next;
            }
            nodes_left--;
        } 
        //and finally append our new initialized and linked layer to the hidden_layers
        
        hidden_layers.append( hidden_layer );
        //finding the next link_layer
        link_layer = hidden_layer; 
        hidden_layer_count--;
    }
    while ( output_node_count > 0 ){
        current_link_layer = link_layer; //link_layer still points at the last hidden layer
        new_node = new Node( output_activation_function, output_activation_function_derivative );
        output_layer.append( new_node );
        while ( ( current_link_layer != nullptr ) && ( current_link_layer->elem != nullptr ) ){
            e = new Edge( current_link_layer->elem, new_node, rand() % 100 ); //initializes a random-weighted ( -100 -- 100 ) edge between link_node and new_node
            current_link_layer = current_link_layer->next;
        }
        --output_node_count;
    }
}
Net::~Net(){
    List <Node> *current_layer_ptr = &input_layer;
    List <List <Node> > *hidden_layers_ptr = &hidden_layers;
    List <Edge> *edge_ptr;
    while ( current_layer_ptr != nullptr ){
        edge_ptr = &( current_layer_ptr->elem->output_list );
        while ( edge_ptr != nullptr ){
            delete edge_ptr->elem;
            edge_ptr = edge_ptr->next;
        }
        delete current_layer_ptr->elem;
        current_layer_ptr = current_layer_ptr->next;
    }
    while ( hidden_layers_ptr != nullptr ){
        current_layer_ptr = hidden_layers_ptr->elem;
        while ( current_layer_ptr != nullptr ){
            edge_ptr = &( current_layer_ptr->elem->output_list );
            while ( edge_ptr != nullptr ){
                delete edge_ptr->elem;
                edge_ptr = edge_ptr->next;
            }
            delete current_layer_ptr->elem;
            current_layer_ptr = current_layer_ptr->next;
        }
        delete hidden_layers_ptr->elem;
        hidden_layers_ptr = hidden_layers_ptr->next;
    }
    current_layer_ptr = &output_layer;
    while ( current_layer_ptr != nullptr ){
        delete current_layer_ptr->elem;
        current_layer_ptr = current_layer_ptr->next;
    }
}
Dlist Net::activate( Dlist input ){
    Dlist ans;
    Dlist *inp = &input;
    List <Node> *current_layer_ptr = &input_layer;
    List <List <Node> > *hidden_layers_ptr = &hidden_layers;
    if ( input.length != input_layer.length )
        std::cerr << "Error: input vector has incorrect length";
    else{
        while ( ( current_layer_ptr != nullptr ) ){
            current_layer_ptr->elem->set_activation_value( inp->elem );
            inp = inp->next;
            current_layer_ptr = current_layer_ptr->next;
        }
        while ( hidden_layers_ptr != nullptr ){
            current_layer_ptr = hidden_layers_ptr->elem;
            while ( current_layer_ptr != nullptr ){
                current_layer_ptr->elem->activate();
                current_layer_ptr->elem->delta = 0;
                current_layer_ptr = current_layer_ptr->next;
            }
            hidden_layers_ptr = hidden_layers_ptr->next;
        }
        current_layer_ptr = &output_layer;
        while ( current_layer_ptr != nullptr ){
            current_layer_ptr->elem->activate();
            current_layer_ptr->elem->delta = 0;
            ans.append( current_layer_ptr->elem->get_activation_value() );
            current_layer_ptr = current_layer_ptr->next;
        }
    }
    return ans;
}
void Net::back_propagate( Dlist input, Dlist ideal, double learning_rate, double momentum ){
    Dlist output = this->activate( input );
    List <Node> *current_layer_ptr = &output_layer;
    List <List <Node> > *hidden_layers_ptr = &hidden_layers;
    Dlist *desired = &ideal;
    List <Edge> *input_list_ptr;
    long long j, i;
    if ( ( output.length == 0 ) || ( desired->length != output.length ) ){
        std::cerr << "Back-propagation terminated";
    }
    else{
        //calculating delta for output layer
        while ( current_layer_ptr != nullptr ){
            input_list_ptr =  &( current_layer_ptr->elem->input_list );
            current_layer_ptr->elem->delta = ( desired->elem - current_layer_ptr->elem->get_activation_value() ) * ( current_layer_ptr->elem->get_derivative_value() );
            while ( input_list_ptr != nullptr ){
                input_list_ptr->elem->prev_weight = input_list_ptr->elem->weight;
                input_list_ptr->elem->from->delta += current_layer_ptr->elem->delta * input_list_ptr->elem->prev_weight;
                input_list_ptr->elem->weight += learning_rate * current_layer_ptr->elem->delta *  input_list_ptr->elem->from->get_activation_value() + ( momentum * input_list_ptr->elem->prev_weight );
                input_list_ptr = input_list_ptr->next;
            }
            current_layer_ptr = current_layer_ptr->next;
        }
        j = hidden_layers.length;
        while ( j-- ){
            i = j;
            hidden_layers_ptr = &hidden_layers;
            while ( ( --i ) && ( j > 0 ) ){
                hidden_layers_ptr = hidden_layers_ptr->next;
            }
            current_layer_ptr = hidden_layers_ptr->elem;
            while ( current_layer_ptr != nullptr ){
                input_list_ptr =  &( current_layer_ptr->elem->input_list );
                current_layer_ptr->elem->delta *= current_layer_ptr->elem->get_derivative_value();
                while ( input_list_ptr != nullptr ){
                    input_list_ptr->elem->prev_weight = input_list_ptr->elem->weight;
                    input_list_ptr->elem->from->delta += current_layer_ptr->elem->delta * input_list_ptr->elem->prev_weight;
                    input_list_ptr->elem->weight += learning_rate * current_layer_ptr->elem->delta *  input_list_ptr->elem->from->get_activation_value() + ( momentum * input_list_ptr->elem->prev_weight );
                    input_list_ptr = input_list_ptr->next;
                }
                current_layer_ptr = current_layer_ptr->next;
            }
        }
    }
}
    