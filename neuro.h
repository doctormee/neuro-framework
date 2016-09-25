
double sgn(double val) {
    return double ( ( 0 < val ) - ( val < 0 ) );
}
/* this is an Edge struct (which is equal to a public class)
It stores index and destination Node pointers and a weight of an edge
*/
struct Edge{
    Node *from, *to;
    double weight;
    Edge( Node *f, Node *t, double w );
};

/* this is a list*/
template <class T>
struct List{
    T *elem;
    List *next;
    List();
    List( T *init );
    void link( T *piece ); // links *piece at the end of a list 
};
/* this is a main class, implementing a node structure with all necessary 
values and methods
*/
class Node{
    double activation_value; //output value
    double ( *activation_function )( double ); //pointer to the activation function
    static double computing_function( List <Edge> inp ); //computing function, is static for all nodes and (in our scenario) is a weighted sum
    // enum n_type{ input, hidden, output, bias }
public:
    List <Edge> input_list, output_list; //these are two Edge lists storing all edges going in and out of the node
    Node(){ activation_value = 0; activation_function = sgn;} //simple constructor, used for making a Bias
    Node( double val, double ( *func )( double ) );
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
    double ( *hidden_activation_function)( double ), double ( *output_activation_function)( double ) ); //node constructior
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
    *f.output_list.link( this );
    *t.input_list.link( this );
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
void List<T>::link( T *piece ) {
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
Node::Node( double val, double ( *func )( double ) ){
    activation_value = val;
    activation_function = func;
}
    