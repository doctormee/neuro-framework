/* this is an Edge struct (which is equal to a public class)
It stores index and destination Node pointers and a weight of an edge
*/
struct Edge{
    Node *to, *from;
    double weight;
    Edge( Node *t, Node *f, double w );
    ~Edge();
};

/* this is a list*/
template <class T>
struct List{
    T *elem;
    List *next;
    List();
    List( T *init );
    ~List();
    void link( T *piece ); // links *piece at the end of a list 
};
/* this is a main class, implementing a node structure with all necessary 
values and methods
*/
class Node{
    double y; //output value
    double ( *F )( double ); //pointer to the activation function
    static double S( List <Edge> inp ); //computing function, is static for all nodes and (in our scenario) is a weighted sum
    // enum n_type{ input, hidden, output, bias }
public:
    List <Edge> input_list, output_list; //these are two Edge lists storing all edges going in and out of the node
    void compute() const { y = F( S( input_list ) ); } //calculates and sets a proper output value 
    void sety( double x ) const { y = x; } //y setter
    double gety() const { return y; } //y getter
};
/* this is a main Net class, which is a neuro-net 
it should provide proper interface
*/ 


class Net{
    List <Node> input_layer, output_layer;
    List <List <Node> > hidden_layers;
public:
    
};
/* this section is solely for defining Edge methods */
Edge::Edge( Node *t, Node *f, double w ){
    to = new Node;
    from = new Node;
    to = t;
    from = f;
    weight = w;
}
Edge::~Edge(){
    delete to;
    delete from;
    w = 0;
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
    elem = new T;
    elem = init; 
}
template <class T>
List<T>::~List(){
    delete elem;
    elem = nullptr;
    if ( next != nullptr ){
        *next.~List();
    }
    delete next;
    next = nullptr;
}
template <class T>
void List<T>::link( T *piece ) {
    List** temp;
    *temp = next;
    while ( *temp != nullptr ){
        *temp = next.next;
    }
    *temp = new List;
    **temp.List( piece );
}
/* this section is solely for defining Node static methods */
double Node::S( List <Edge> inp )
{
    double val = 0;
    while ( inp.next != nullptr ){
        val += inp.*elem.*from.get_y() * inp.*elem.weight; 
        inp = inp.*next;
    }
    return val;
}
    