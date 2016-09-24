/* this is an Edge struct (which is equal to a public class)
It stores index and destination Node pointers and a weight of an edge
*/
struct Edge{
    Node *to, *from;
    double weight;
};

/* this is a specific list of edges */
struct E_list{
    Edge *edge;
    E_list *next;
};
/* this is a main class, implementing a node structure with all necessary 
values and methods
*/
class Node{
    double y; //output value
    double F( double x ); //activation
    static double S( E_list *inp ) //computing function, is static for all nodes and (in our scenario) is a weighted sum
    {
        double val = 0;
        if ( *inp != NULL ){
            while ( *inp.next != NULL ){
                val += *inp.edge.from.get_y() * *inp.edge.weight; 
                *inp = *inp.next;
            }
        }
        return val;
    }
    E_list input_list, output_list; //these are two E_lists storing all edges going in and out of the node
    // enum n_type{ input, hidden, output, bias }
public:
    static void compute(){
        y = F( S( input_list ) );
    }
    static void sety( double x )
    {
        y = x;
    }
    static double gety(){
        return y;
    }
};
/* this is a Layer structure, used by Net */
struct Layer{
    Node *node;
    Layer *next;
};
/* this is a main Net class, which is a neuro-net 
it should provide proper interface
*/
class Net{
    layer input_layer, output_layer;
    struct hidden_layers{
        
    };
}