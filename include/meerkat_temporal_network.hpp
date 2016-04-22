/* meerkat temporal network.
 *
 * A class for storing and managing the dynamics of a temporal network.
 *
 * Copyright (c) 2016, Enys Mones.
 */

#ifndef MEERKAT_TEMPORAL_NETWORK_HPP
#define MEERKAT_TEMPORAL_NETWORK_HPP

#include "stdlib.h"
#include "stdio.h"
#include <string>
#include <vector>
#include <map>
#include "meerkat_file_manager.hpp"
#include "meerkat_logger.hpp"

namespace meerkat {

class mk_temporal_network
{
private:
    struct __node;

    /***********************
     * Internal structures *
     ***********************/
    /**
     * @brief The edge struct:  Encapsulates all edge-related operations.
     *                          Since the dynamics is based on the edge temporality,
     *                          this struct contains time setting and update methods.
     */
    struct __edge {
        __node *_ptr;                   // Pointer to the neighboring node.
        int _vectorIndex;               // Current index in the starting times and durations
                                        // vectors.
        int _timeIndex;                 // Current global time.
        int _activeTime;                // Current active time of the edge.
        int _waitingTime;               // Waiting time to the next appearance.
        std::vector<int> _starts;       // Starting times of the edge.
        std::vector<int> _durations;    // Durations of the edge in the different appearances.

        /**
         * @brief edge       Constructor of the edge, sets up internal temporal vectors based
         *                   on an activity pattern.
         * @param ptr_       Pointer to the neighboring node.
         * @param activity_  Activity pattern of the edge. An element in the activity_ vector
         *                   is true if and only if the edge is active at that time index.
         */
        __edge( __node* ptr_, std::vector<bool> &activity_ );
    };

    /**
     * @brief The node struct  Contains convenience node-related opreations.
     */
    struct __node {
        int _id;                                  // Vector id of the node in the network.
        std::string _label;                       // Label of the node.
        std::vector<__edge*> _neighbors;          // Vector of pointers to the incident edges.
        std::vector<__node*> _activeNeighbors;    // Vector of pointers to the active neighbors.

        /**
         * @brief node    Constructor of the node.
         * @param id_     Vector id in the network.
         * @param label_  Node label.
         */
        __node( const int id_, const std::string label_ );

        /**
         * @brief add_neighbor  Adds a neighbor.
         * @param edge_         Pointer to the edge that contains the neighbor's pointer.
         * @return              True if neighbor could be added, false otherwise.
         */
        bool add_neighbor( __edge *edge_ );

        /**
         * @brief set_clock  Sets clock time.
         * @param time_      Start time of the clock to set.
         * @return           True if clock time could be set, false otherwise.
         */
        bool set_clock( const int time_ );

        /**
         * @brief update_clock  Updates clock time by one step.
         * @note                When clock time reaches end of time, it is reset to start
         *                      autmatically.
         */
        void update_clock();
    };


    /********************
     * Member variables *
     ********************/
    std::vector<__node*> _nodes;                // Vector containing pointers to the nodes.
    std::map<std::string, int> _labelsHash;     // Mapping from label to id in the node vector.
    int _currentTime;                           // Current time index.
    int _maxTime;                               // Maximum time index.
    mk_logger _log;                             // Internal logger class for log messages.


    /********************
     * Internal methods *
     ********************/
    /**
     * @brief _id_node_id_valid  Checks if a given node id is valid.
     * @param nodeId_            Node id to check.
     * @return                   True if node id is valid, false otherwise.
     */
    bool _is_node_id_valid( int nodeId_ ) const;

    /**
     * @brief _is_there_edge  Checks whether a given edge is present (regardless of activity).
     * @param node1Id_        Vector id of the first terminal node of the edge.
     * @param node2Id_        Vector id of the second terminal node of the edge.
     * @return                True if edge with vector ids exists, false otherwise.
     * @note                  This method does not check against vectir id validity.
     */
    bool _is_there_edge( const int node1Id_, const int node2Id_ ) const;

    /**
     * @brief add_node  Adds a node to the network.
     * @param label_    Label of the node to add.
     * @return          True if node could be added, false otherwise.
     */
    bool _add_node( const std::string label_ );

    /**
     * @brief _add_edge  Adds an edge to the network.
     * @param node1Id_   Vector id of the first terminal node of the edge.
     * @param node2Id_   Second terminal node of the edge.
     * @param activity_  Activity pattern of the edge to add.
     * @return           True if edge could be added, false otherwise.
     */
    bool _add_edge( const int node1Id_, const int node2Id_, std::vector<bool> &activity_ );


public:
    /***************************
     * Constructor, destructor *
     ***************************/
    /**
     * @brief mk_temporal_network  Constructor.
     */
    mk_temporal_network();

    /**
     * @brief ~mk_temporal_network  Destructor.
     */
    ~mk_temporal_network();


    /***************************
     * Structure manipulations *
     ***************************/
    /**
     * @brief create        Creates the temporal network based on a temporal edge list.
     * @param filename_     File containing the temporal edges in the following CSV structure:
     *                      node1 node2 time duration
     * @param timeWindow_   Time window of the temporal egde list (size of a single time step).
     * @param reverseTime_  If true, network is read from the file in reversed time.
     * @return              True if network could be created from file, false otherwise.
     */
    bool create( const std::string filename_,
                 int timeWindow_ = 1,
                 bool reverseTime_ = false );

    /**
     * @brief create        Creates the temporal network based on nodes and edge lists.
     * @param nodesFile_    File containing the node labels.
     * @param edgesFile_    File containing the temporal edges in the following CSV structure:
     *                      node1 node2 time duration
     * @param timeWindow_   Time window of the temporal egde list (size of a single time step).
     * @param reverseTime_  If true, network is read from the file in reversed time.
     * @return              True if network could be created from file, false otherwise.
     */
    bool create( const std::string nodesFile_,
                 const std::string edgesFile_,
                 int timeWindow_ = 1,
                 bool reverseTime_ = false );

    /**
     * @brief destroy  Destroys network.
     * @return         True if network could be destroyed and memory was freed, false otherwise.
     */
    bool destroy();


    /*****************
     * Local queries *
     *****************/
    /**
     * @brief label    Returns a constant pointer to the label of a given node.
     * @param nodeId_  Vector id of the node.
     * @return         Constant pointer to the label of the node if vector id is valid,
     *                 NULL pointer otherwise.
     */
    const std::string* label( int nodeId_ ) const;

    /**
     * @brief _node_id  Returns the vector id for a given label.
     * @param label_    Label of the node to query.
     * @return          Vector id of the node if label is valid, -1 otherwise.
     */
    int node_id( std::string label_ ) const;

    /**
     * @brief degree   Returns the total degree of a node (regardless of activity).
     * @param nodeId_  Vector id of the node.
     * @return         Number of neighbors.
     */
    int degree( int nodeId_ ) const;

    /**
     * @brief active_degree  Returns the number of active neighbors of a node.
     * @param nodeId_        Vector id of the node.
     * @return               Number of active neighbors.
     */
    int active_degree( int nodeId_ ) const;

    /**
     * @brief neighbor     Returns the vector id of a neighbor of a node.
     * @param nodeId_      Vector id of the node to query.
     * @param neighborId_  Neighbor id of the neighbor to query.
     * @return             Vector id of the neighbor if both the node and the neighbor exist,
     *                     -1 otherwise.
     */
    int neighbor( int nodeId_, int neighborId_ ) const;

    /**
     * @brief active_neighbor  Returns the vector id of an active neighbor of a node.
     * @param nodeId_          Vector id of the node.
     * @param neighborId_      Neighbor id of the neighbor to query,
     * @return                 Vector id of the active neighbor if both the node and the neigbor
     *                         exist, -1 otherwise.
     */
    int active_neighbor( int nodeId_, int neighborId_ ) const;

    /**
     * @brief active_edges  Queries currently active edges.
     * @param edges_        Edges will be stored here as integer pairs.
     */
    void active_edges( std::vector<std::pair<int, int> > &edges_ ) const;



    /***************************
     * Global queries *
     ***************************/
    /**
     * @brief timeId  Returns current time.
     * @return        Current time.
     */
    int time() const;

    /**
     * @brief maxTime  Returns max time.
     * @return         Max time.
     */
    int maxTime() const;

    /**
     * @brief order  Returns the number of nodes.
     * @return       Numnber of nodes.
     */
    int order() const;

    /**
     * @brief size  Returns number of static edges (regardless of activity).
     * @return      Number of all possible edges.
     */
    int size() const;


    /**
     * @brief size_t  Returns number of temporal edges.
     * @return        Number of temporal edges.
     */
    int size_temporal() const;

    /**
     * @brief active_size  Returns number of currently active edges.
     * @return             Number of active edges.
     */
    int active_size() const;

    /**
     * @brief set_clock  Sets clock time.
     * @param time_      Initial time to set the clock to.
     */
    void set_clock( const int time_ );

    /**
     * @brief update_clock  Updates clock by one time step.
     */
    void update_clock();
};

}

#endif // MK_TEMPORAL_NETWORK_HPP
