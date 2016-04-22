#include "meerkat_temporal_network.hpp"


meerkat::mk_temporal_network::__edge::__edge( __node *ptr_, std::vector<bool> &activity_ )
{
    // Set pointer
    _ptr = ptr_;

    // Set time vectors
    int length = (int)activity_.size();
    int s, d = 0;
    for( int i=0; i<length; i++ )
    {
        // Active time index found
        if( activity_[i] )
        {
            // Take start time and start counting duration
            s = i;
            d = 1;

            // Go through activity elements
            while( true )
            {
                i++;
                // If edge is active, increment duration
                if( activity_[i] )
                    d++;
                // Otherwise, take start time and duration
                else
                {
                    _starts.push_back(s);
                    _durations.push_back(d);
                    break;
                }
            }
        }
    }

    // Set final time step with duration of -1.
    // The invalid duration marks the end of the clock, at which it is set back to start.
    _starts.push_back( length );
    _durations.push_back( -1 );
}

meerkat::mk_temporal_network::__node::__node( const int id_, const std::string label_ )
{
    _id = id_;
    _label = label_;
}

bool meerkat::mk_temporal_network::__node::add_neighbor( __edge *edge_ )
{
    // Check if edge or neighbor is null
    if( edge_ == NULL || edge_->_ptr == NULL )
        return false;
    else
    {
        _neighbors.push_back( edge_ );
        return true;
    }
}

bool meerkat::mk_temporal_network::__node::set_clock( const int time_ )
{
    // Check if time is valid (inside total time interval)
    if( (int)_neighbors.size() > 0 )
    {
        if( time_ >= _neighbors[0]->_starts.back() )
            return false;
    }

    int deg = (int)_neighbors.size();
    __edge *edge = NULL;
    for( int i=0; i<deg; i++ )
    {
        edge = _neighbors[i];

        // Time index
        edge->_timeIndex = time_;

        // Vector index
        edge->_vectorIndex = 0;
        while( edge->_starts[edge->_vectorIndex] <= edge->_timeIndex )
        {
            edge->_vectorIndex++;
        }

        // Active time
        if( edge->_vectorIndex == 0 )
            edge->_activeTime = 0;
        else
        {
            edge->_activeTime = edge->_durations[edge->_vectorIndex-1]
                    - (edge->_timeIndex - edge->_starts[edge->_vectorIndex-1]);
            if( edge->_activeTime < 0 )
                edge->_activeTime = 0;
        }
        edge->_waitingTime = edge->_starts[edge->_vectorIndex] - edge->_timeIndex;
    }

    // Also update active neighbors
    _activeNeighbors.clear();
    for( int j=0; j<deg; j++ )
    {
        if( _neighbors[j]->_activeTime > 0 )
            _activeNeighbors.push_back( _neighbors[j]->_ptr );
    }

    return true;
}

void meerkat::mk_temporal_network::__node::update_clock()
{
    int deg = (int)_neighbors.size();
    __edge *edge;
    for( int i=0; i<deg; i++ )
    {
        edge = _neighbors[i];

        // Increment time
        edge->_timeIndex++;

        // If active time is not zero, decrease
        if( edge->_activeTime > 0 )
            edge->_activeTime--;

        // Decrease waiting time
        edge->_waitingTime--;
        if( edge->_waitingTime == 0 )
        {
            edge->_activeTime = edge->_durations[edge->_vectorIndex];
            edge->_vectorIndex++;
            edge->_waitingTime = edge->_starts[edge->_vectorIndex] - edge->_timeIndex;
        }
    }

    // Update active neighbors
    _activeNeighbors.clear();
    for( int j=0; j<deg; j++ )
    {
        if( _neighbors[j]->_activeTime > 0 )
            _activeNeighbors.push_back(_neighbors[j]->_ptr);
    }
}


int meerkat::mk_temporal_network::node_id( std::string label_ ) const
{
    if( _labelsHash.find(label_) != _labelsHash.end() )
        return _labelsHash.at(label_);
    else
        return -1;
}

bool meerkat::mk_temporal_network::_is_node_id_valid(int nodeId_) const
{
    return (nodeId_ >= 0 && nodeId_ < (int)_nodes.size());
}

bool meerkat::mk_temporal_network::_is_there_edge( const int node1Id_, const int node2Id_ ) const
{
    int deg = (int)_nodes[node1Id_]->_neighbors.size();
    for( int j=0; j<deg; j++ )
    {
        if( _nodes[node1Id_]->_neighbors[j]->_ptr == _nodes[node2Id_] )
            return true;
    }
    return false;
}

bool meerkat::mk_temporal_network::_add_node( const std::string label_ )
{
    // Check label
    if( label_ == "" )
    {
        _log.w( "_add_node", "label is empty" );
        return false;
    }
    else
    {
        std::pair<std::map<std::string, int>::iterator, bool> added;
        added = _labelsHash.insert(
                    std::pair<std::string, int>( std::string(label_), (int)_labelsHash.size() )
                    );
        if( added.second )
        {
            _nodes.push_back( new __node( (int)_labelsHash.size()-1, label_ ) );
            return true;
        }
        else
            return false;
    }
}

bool meerkat::mk_temporal_network::_add_edge( const int node1Id_, const int node2Id_,
                                              std::vector<bool> &activity_ )
{
    // Check node ids
    if( !_is_node_id_valid(node1Id_) || !_is_node_id_valid(node2Id_) )
    {
        _log.w( "_add_edge", "invalid node id(s)" );
        return false;
    }
    else
    {
        // No multiple and no self-edges are allowed
        if( node1Id_ != node2Id_ && !_is_there_edge(node1Id_, node2Id_) )
        {
            // Add neighbors
            __edge *neighbor = new __edge( _nodes[node2Id_], activity_ );
            _nodes[node1Id_]->add_neighbor( neighbor );
            neighbor = new __edge( _nodes[node1Id_], activity_ );
            _nodes[node2Id_]->add_neighbor( neighbor );
            return true;
        }
        else
            return false;
    }
}

meerkat::mk_temporal_network::mk_temporal_network()
{
    _currentTime = 0;
    _maxTime = 0;
    _log.tag( "mk_temporal_network" );
}

meerkat::mk_temporal_network::~mk_temporal_network()
{
    destroy();
}

bool meerkat::mk_temporal_network::create( const std::string filename_,
                                           int timeWindow_,
                                           bool reverseTime_ )
{
    /// Try to open file.
    mk_file_manager fm;
    if( !fm.read(filename_) )
    {
        _log.e( "create", "no such file: '%s'", filename_.c_str() );
        return false;
    }

    /// Read number of rows.
    int rows = fm.rows();

    /// Determine min and max times (also take lines)
    char line[1024] = {""};
    std::vector<std::string> lines( rows, "" );
    char node1Label[128] = {""}, node2Label[128] = {""};
    unsigned long edgeTime = 0, edgeDuration = 0;

    // Eat up header
    fm.get_line();

    // Set start and end timestamp initial values
    strcpy(line, fm.get_line().c_str());
    sscanf( line, "%s %s %lu %lu", node1Label, node2Label, &edgeTime, &edgeDuration );
    unsigned long startTimestamp = edgeTime;
    unsigned long endTimestamp = edgeTime;
    lines[0] = std::string( line );
    // Read rest of the rows
    for( int r=1; r<rows; r++ )
    {
        strcpy(line, fm.get_line().c_str());
        sscanf( line, "%s %s %lu %lu", node1Label, node2Label, &edgeTime, &edgeDuration );
        lines[r] = std::string( line );

        // Get min, max and window
        if( edgeTime < startTimestamp )
            startTimestamp = edgeTime;
        if( edgeTime + edgeDuration > endTimestamp )
            endTimestamp = edgeTime + edgeDuration;
    }
    // Calculate number of time steps
    _maxTime = int((endTimestamp - startTimestamp) / (unsigned long)timeWindow_);
    if( _maxTime == 0 )
        _maxTime = 1;
    _log.i( "create", "start time:        %lu", startTimestamp );
    _log.i( "create", "end time:          %lu", endTimestamp );
    _log.i( "create", "max time index:    %i", _maxTime );

    /// Add nodes
    int numLines = (int)lines.size();
    for( int i=0; i<numLines; i++ )
    {
        sscanf( lines[i].c_str(), "%s %s %lu %lu",
                node1Label, node2Label, &edgeTime, &edgeDuration );
        _add_node( std::string(node1Label) );
        _add_node( std::string(node2Label) );
    }
    _log.i( "create", "number of nodes:   %i", order() );

    /// Read in activity patterns
    std::map<std::pair<int, int>, std::vector<bool> > activities;
    std::map<std::pair<int, int>, std::vector<bool> >::iterator it;
    int node1Id, node2Id, sNode1Id, sNode2Id, timeIdx, timeDur;
    for( int i=0; i<numLines;i++ )
    {
        // Get node ids and time index
        sscanf( lines[i].c_str(), "%s %s %lu %lu",
                node1Label, node2Label, &edgeTime, &edgeDuration );
        node1Id = node_id( node1Label );
        node2Id = node_id( node2Label );
        timeIdx = int((edgeTime-startTimestamp) / (unsigned long)timeWindow_);
        timeDur = int(edgeDuration / (unsigned long)timeWindow_);
        if( timeDur == 0 )
            timeDur = 1;

        // Reverse if it is enabled
        if( reverseTime_ )
            timeIdx = _maxTime - (timeIdx + timeDur) + 1;

        // Sorted node ids
        sNode1Id = node1Id < node2Id ? node1Id : node2Id;
        sNode2Id = node1Id < node2Id ? node2Id : node1Id;

        // Add activity if edge is new
        if( activities.find( std::pair<int, int>(sNode1Id, sNode2Id) ) == activities.end() )
            activities[std::pair<int, int>(sNode1Id, sNode2Id)] =
                    std::vector<bool>( _maxTime+1, false );
        // Set timestamp to true
        for( int ti=0; ti<timeDur; ti++ )
            activities[std::pair<int, int>(sNode1Id, sNode2Id)][timeIdx+ti] = true;
    }

    /// Add edges
    for( it=activities.begin(); it!=activities.end(); it++ )
    {
        node1Id = it->first.first;
        node2Id = it->first.second;
        _add_edge( node1Id, node2Id, it->second );
    }
    _log.i( "create", "number of edges:   %i", size() );

    /// Init time
    set_clock( 0 );
    return true;
}

bool meerkat::mk_temporal_network::create( const std::string nodesFile_,
                                           const std::string edgesFile_,
                                           int timeWindow_,
                                           bool reverseTime_ )
{
    /// Check if nodes file exists
    mk_file_manager fn, fe;
    if( !(fn.read(nodesFile_)) )
    {
        _log.e( "create", "no such file: '%s'", nodesFile_.c_str() );
        return false;
    }
    if( !(fe.read(edgesFile_)) )
    {
        _log.e( "create", "no such file: '%s'", edgesFile_.c_str() );
        return false;
    }

    /// Read nodes
    int rows = fn.rows();
    char line[1024] = {""}, nodeLabel[128] = {""};
    fn.get_line();
    for( int r=1; r<rows; r++ )
    {
        sscanf( fn.get_line().c_str(), "%s", nodeLabel );
        _add_node( std::string(nodeLabel) );
    }
    _log.i( "create", "number of nodes:   %i", order() );

    /// Read edges
    rows = fe.rows() - 1;

    // Determine min and max times (also take lines)
    std::vector<std::string> lines( rows, "" );
    char node1Label[128] = {""}, node2Label[128] = {""};
    unsigned long edgeTime = 0, edgeDuration = 0;

    // Eat up header
    fe.get_line();

    // Set start and end timestamp initial values
    strcpy( line, fe.get_line().c_str() );
    sscanf( line, "%s %s %lu %lu", node1Label, node2Label, &edgeTime, &edgeDuration );
    unsigned long startTimestamp = edgeTime;
    unsigned long endTimestamp = edgeTime;
    lines[0] = std::string( line );
    // Read rest of the rows
    for( int r=1; r<rows; r++ )
    {
        strcpy( line, fe.get_line().c_str() );
        sscanf( line, "%s %s %lu %lu", node1Label, node2Label, &edgeTime, &edgeDuration );
        lines[r] = std::string( line );

        // Get min, max and window
        if( edgeTime < startTimestamp )
            startTimestamp = edgeTime;
        if( edgeTime + edgeDuration > endTimestamp )
            endTimestamp = edgeTime + edgeDuration;
    }
    // Calculate number of time steps
    _maxTime = int((endTimestamp - startTimestamp) / (unsigned long)timeWindow_);
    _log.i( "create", "start time:        %lu", startTimestamp );
    _log.i( "create", "end time:          %lu", endTimestamp );
    _log.i( "create", "max time index:    %i", _maxTime );

    /// Read in activity patterns
    int numLines = (int)lines.size();
    std::map<std::pair<int, int>, std::vector<bool> > activities;
    std::map<std::pair<int, int>, std::vector<bool> >::iterator it;
    int node1Id, node2Id, sNode1Id, sNode2Id, timeIdx, timeDur;
    for( int i=0; i<numLines;i++ )
    {
        // Get node ids and time index
        sscanf( lines[i].c_str(), "%s %s %lu %lu",
                node1Label, node2Label, &edgeTime, &edgeDuration );
        node1Id = node_id( node1Label );
        node2Id = node_id( node2Label );
        timeIdx = int((edgeTime-startTimestamp) / (unsigned long)timeWindow_);
        timeDur = int(edgeDuration / (unsigned long)timeWindow_);

        // Reverse if it is enabled
        if( reverseTime_ )
            timeIdx = _maxTime - (timeIdx + timeDur) + 1;

        // Sorted node ids
        sNode1Id = node1Id < node2Id ? node1Id : node2Id;
        sNode2Id = node1Id < node2Id ? node2Id : node1Id;

        // Add activity if edge is new
        if( activities.find( std::pair<int, int>(sNode1Id, sNode2Id) ) == activities.end() )
            activities[std::pair<int, int>(sNode1Id, sNode2Id)] =
                    std::vector<bool>( _maxTime, false );
        // Set timestamp to true
        for( int ti=0; ti<timeDur; ti++ )
            activities[std::pair<int, int>(sNode1Id, sNode2Id)][timeIdx+ti] = true;
    }

    /// Add edges
    for( it=activities.begin(); it!=activities.end(); it++ )
    {
        node1Id = it->first.first;
        node2Id = it->first.second;
        _add_edge( node1Id, node2Id, it->second );
    }
    _log.i( "create", "number of edges:   %i", size() );

    /// Init time
    set_clock( 0 );
    return true;
}

bool meerkat::mk_temporal_network::destroy()
{
    int o = order();
    if( o > 0 )
    {
        int deg;
        for( int i=0; i<o; i++ )
        {
            deg = degree(i);
            for( int j=0; j<deg; j++ )
                delete _nodes[i]->_neighbors[j];
            delete _nodes[i];
        }
        _nodes.clear();
        _labelsHash.clear();
        _log.i( "destroy", "network is destroyed" );
        return true;
    }
    else
    {
        return false;
    }
}

const std::string *meerkat::mk_temporal_network::label( int nodeId_ ) const
{
    // Check node id
    if( _is_node_id_valid(nodeId_) )
        return &_nodes[nodeId_]->_label;
    else
    {
        _log.w( "label", "invalid node id" );
        return NULL;
    }
}

int meerkat::mk_temporal_network::degree( int nodeId_ ) const
{
    // Check node id
    if( _is_node_id_valid(nodeId_) )
        return (int)_nodes[nodeId_]->_neighbors.size();
    else
    {
        _log.w( "degree", "invalid node id" );
        return 0;
    }
}

int meerkat::mk_temporal_network::active_degree( int nodeId_ ) const
{
    // Check node id
    if( _is_node_id_valid(nodeId_)  )
        return (int)_nodes[nodeId_]->_activeNeighbors.size();
    else
    {
        _log.w( "active_degree", "invalid node id" );
        return 0;
    }
}

int meerkat::mk_temporal_network::neighbor( int nodeId_, int neighborId_ ) const
{
    // Check node id
    if( !_is_node_id_valid(nodeId_) )
    {
        _log.w( "neighbor", "invalid node id" );
        return -1;
    }

    // Check neighbor id
    if( neighborId_ < 0 || neighborId_ >= degree(nodeId_) )
    {
        _log.w( "neighbor", "invalid neighbor id" );
        return -1;
    }

    return _nodes[nodeId_]->_neighbors[neighborId_]->_ptr->_id;
}

int meerkat::mk_temporal_network::active_neighbor( int nodeId_, int neighborId_ ) const
{
    // Check node id
    if( !_is_node_id_valid(nodeId_) )
    {
        _log.w( "active_neighbor", "invalid node id" );
        return -1;
    }

    // Check neighbor id
    if( neighborId_ < 0 || neighborId_ >= degree(nodeId_) )
    {
        _log.w( "active_neighbor", "invalid neighbor id" );
        return -1;
    }

    return _nodes[nodeId_]->_activeNeighbors[neighborId_]->_id;
}

void meerkat::mk_temporal_network::active_edges( std::vector<std::pair<int, int> > &edges_ ) const
{
    edges_.resize( active_size(), std::pair<int, int>() );
    int o = order(), deg, ni, k = 0;
    for( int i=0; i<o; i++ )
    {
        deg = active_degree(i);
        for( int j=0; j<deg; j++ )
        {
            ni = active_neighbor(i, j);
            if( ni > i )
            {
                edges_[k] = std::pair<int, int>( i, ni );
                k++;
            }
        }
    }
}

int meerkat::mk_temporal_network::time() const
{
    return _currentTime;
}

int meerkat::mk_temporal_network::maxTime() const
{
    return _maxTime;
}

int meerkat::mk_temporal_network::order() const
{
    return (int)_nodes.size();
}

int meerkat::mk_temporal_network::size() const
{
    int o = order(), s = 0;
    for( int i=0; i<o; i++ )
        s += degree(i);
    return s / 2;
}

int meerkat::mk_temporal_network::size_temporal() const
{
    int o = order(), s = 0, deg, length;
    for( int i=0; i<o; i++ )
    {
        deg = degree(i);
        for( int j=0; j<deg; j++ )
        {
            length = (int)_nodes[i]->_neighbors[j]->_durations.size();
            for( int e=0; e<length; e++ )
            {
                if( _nodes[i]->_neighbors[j]->_durations[e] > 0 )
                    s += _nodes[i]->_neighbors[j]->_durations[e];
            }
        }
    }
    return s/2;
}

int meerkat::mk_temporal_network::active_size() const
{
    int o = order(), s = 0;
    for( int i=0; i<o; i++ )
        s += active_degree(i);
    return s / 2;
}

void meerkat::mk_temporal_network::set_clock(const int time_)
{
    int o = order();
    for( int i=0; i<o; i++ )
        _nodes[i]->set_clock( time_ );
    _currentTime = time_;
}

void meerkat::mk_temporal_network::update_clock()
{
    int o = order();
    _currentTime = (_currentTime+1) % _maxTime;
    if( _currentTime == 0 )
        set_clock( 0 );
    else
    {
        for( int i=0; i<o; i++ )
            _nodes[i]->update_clock();
    }
}

