#include <climits>
#include <cstdint>
#include <iostream>
#include <list>
#include <vector>
#include "graph_helpers.h"

using namespace std;

/**
 * Performs a depth first search from a given vector, while keeping track of
 * the order that vertices are found. 
 * @param vertex the number of the vertex to perform the search on
 * @param visited a reference to a vector of which vertices have been visited
 * @param circuit a reference to a vecotr of the order of visited vertices
 * @param mst a graph of the mst which tells us which of edges are available
 * to use in dfs
 */
void dfs (size_t vertex, vector <bool > & visited, vector < size_t > & circuit,
	  vector< list< Edge >> mst);

/*
 * Reads in a graph, finds a MST and then uses a twice around the tree algorithm
 * to approximate the traveling salesman problem. Requires a file of graph 
 * information as input.
 */
int main ()
{

  vector< list < Edge >> graph;
  vector< Edge > all_edges;

  read_adjacency_lists( graph, all_edges);
  print_graph( graph );

  // implement prim's algorithm for mst, hard-coded to start at vertex 0
  vector< list< Edge >> mst;
  vector< bool > known;

  for( size_t i = 0; i < graph.size(); i++ )
  {
    list< Edge > empty_list;
    mst.push_back( empty_list );
    known.push_back( false );
  }

  known.at( 0 ) = true;

  Edge max_weight_edge;
  max_weight_edge.start_vertex = UINT_MAX;
  max_weight_edge.end_vertex = UINT_MAX;
  max_weight_edge.weight = UINT_MAX;

  // do this once for each vertex after 0
  // i is irrelevant
  for( size_t i = 1; i < graph.size(); i++ )
  {
    // get shortest-distance edge from known to unknown vertices
    Edge shortest_edge = max_weight_edge;
    for( auto edge : all_edges )
    {
      if( known.at( edge.start_vertex )
          && !known.at( edge.end_vertex)
          && edge < shortest_edge )
      {
        shortest_edge = edge;
      }
    }

    // add this edge and its reverse to mst
    Edge other_direction;
    other_direction.start_vertex = shortest_edge.end_vertex;
    other_direction.end_vertex = shortest_edge.start_vertex;
    other_direction.weight = shortest_edge.weight;

    mst.at( shortest_edge.start_vertex ).push_back( shortest_edge );
    mst.at( shortest_edge.end_vertex ).push_back( other_direction );
    known.at( shortest_edge.end_vertex ) = true;

    // go through and remove all edges from a known to a known vertex
    // this is not essential but results in fewer basic operations
    vector< Edge > edges_to_keep;
    for( auto edge : all_edges )
    {
      if( !known.at( edge.start_vertex ) || !known.at( edge.end_vertex ))
      {
        edges_to_keep.push_back( edge );
      }
    }
    all_edges = edges_to_keep;
  }

  //twice around the tree algorithm
  vector < bool > visited;
  vector < size_t > circuit;
  for( size_t i = 0; i < graph.size(); i++ )
  {
    visited.push_back( false );
  }
 
  dfs (0, visited, circuit,mst);   
  vector <size_t> better_circuit;
  for (size_t i = 0; i < circuit.size(); i++)
    {
      bool repeat = false;
      for (size_t j = 0; j < better_circuit.size(); j++)
	{
	  if (circuit.at(i) == better_circuit.at(j))
	    repeat = true;
	}
      if (!repeat)
	{
	  better_circuit.push_back(circuit.at(i));
	}
    }
  circuit.push_back(0);
  

  uint length = 0;
  for (size_t i = 0; i < better_circuit.size() - 1; i++)
    {
      size_t end_vertex = better_circuit.at(i+1);
      for (auto edge : graph.at(i))
	{
	  if (edge.end_vertex  == end_vertex)
	    length += edge.weight;
	}
    }

  cout << "the mst:" << endl;
  print_graph( mst );
  
  cout << "Hamiltonian circuit: " << better_circuit.at(0);
  for (size_t i = 1; i < better_circuit.size(); i++)
    {
      cout << ", " << better_circuit.at(i);
    }


  cout << "\ncircuit length: " << length << endl;
  return 0;
}


void dfs (size_t  vertex, vector <bool > & visited, vector < size_t > & circuit,
	  vector< list< Edge >> mst)
{
  visited.at(vertex) = true;
  circuit.push_back(vertex);
  bool back = true;
  for ( auto edge : mst.at(vertex) )
    {
      cout << "vertex: " << vertex << "edge.end_vertex: " << edge.end_vertex << endl;
      if (!visited.at(edge.end_vertex))
	{
	  dfs(edge.end_vertex, visited, circuit, mst);
	  back = false;
	}
    }
  if (back && vertex != mst.size()-1)
    {
      circuit.push_back(mst.at(vertex).front().end_vertex);
    }

 cout << "Hamiltonian circuit: " << circuit.at(0);
  for (size_t i = 1; i < circuit.size(); i++)
    {
      cout << ", " << circuit.at(i);
    }
  cout << endl;
}
