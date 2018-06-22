#include "triangulation_2.hpp"
#include "ra/kernel.hpp"
#include <CGAL/Cartesian.h>
#include <CGAL/Cartesian.h>
#include <string>
#include <iostream>
#include <map>
#include <utility>

using Kernel = CGAL::Cartesian<double>;
using Triangulation = trilib::Triangulation_2<Kernel>;
using Suspect_Map = std::map<Triangulation::Halfedge_const_handle, int>;

Suspect_Map suspects;

bool is_flippable(Triangulation::Halfedge_const_handle halfedge)
{
  return !(halfedge->is_border() || halfedge->opposite()->is_border());
}


void mark_edges(Triangulation::Halfedge_const_handle edge)
{
  if(is_flippable(edge))
  {
    suspects[edge] = 0;
    suspects[edge->opposite()] = 0;
  }
}

//find neighboring faces, return edges associated
void mark_potential_suspects(const Triangulation& tri, Triangulation::Halfedge_const_handle flippedEdge)
{
	for (auto faceIter = tri.faces_begin(); faceIter != tri.faces_end();
	  ++faceIter) {
		Triangulation::Halfedge_const_handle h = faceIter->halfedge();
		Triangulation::Vertex_const_handle a = h->vertex();
		Triangulation::Vertex_const_handle b = h->opposite()->vertex();
		Triangulation::Vertex_const_handle c = h->next()->vertex();
		std::cout << "face " << a->point() << ' ' << b->point() << ' '
		  << c->point() << '\n';

      if(h == flippedEdge)
      {
        mark_edges(h->next());
        mark_edges(h->next()->next());
        mark_edges(h->opposite()->next());
        mark_edges(h->opposite()->next()->next());
      }
	}
}

bool is_everythig_optimal(Suspect_Map& suspects)
{
  int x = 1;
  
  for( auto const& [key, val] : suspects )
  {
      x &= val;
  }
  return x == 1;
}


int main(int argc, char** argv)
{
  // lop<double>();
  //map to tag the suspects
  Triangulation tri(std::cin);
  Kernel::Vector_2 u(1,0);
  Kernel::Vector_2 v(1,1);

  ra::geometry::Kernel<double> k;

	// std::cout << "number of vertices " << tri.size_of_vertices() << '\n';
	// std::cout << "number of faces " << tri.size_of_faces() << '\n';
	// std::cout << "number of edges " << tri.size_of_edges() << '\n';
	// std::cout << "number of halfedges " << tri.size_of_halfedges() << '\n';

  // std::cout << "edges in triangulation:\n";
  //all of them are suspects  
	for (auto halfedgeIter = tri.halfedges_begin(); halfedgeIter !=
	    tri.halfedges_end(); ++++halfedgeIter) {
		Triangulation::Vertex_const_handle a = halfedgeIter->vertex();
		Triangulation::Vertex_const_handle b =   
		halfedgeIter->opposite()->vertex();
    if(is_flippable(halfedgeIter))
    {
      suspects[halfedgeIter] = 0;
    }
	}
  
    // for each edge
  while(!is_everythig_optimal(suspects))
  {
    for (auto halfedgeIter = tri.halfedges_begin(); halfedgeIter != tri.halfedges_end(); ++++halfedgeIter) {
      Triangulation::Vertex_const_handle a = halfedgeIter->vertex();
      Triangulation::Vertex_const_handle b =   
      halfedgeIter->opposite()->vertex();


      auto pd = halfedgeIter->opposite()->next()->vertex()->point();
      auto pa = halfedgeIter->vertex()->point();
      auto pc = halfedgeIter->opposite()->vertex()->point();
      auto pb = halfedgeIter->next()->vertex()->point();
      
      if(k.is_locally_pd_delaunay_edge(pa,pb,pc,pd,u,v) && is_flippable(halfedgeIter)) 
      {
        // std::cout << "faces in You are inside:\n";
        
          // tri.flip_edge(halfedgeIter);
          suspects[halfedgeIter] = 1;
          suspects[halfedgeIter->opposite()] = 1;
          // mark_potential_suspects(tri,halfedgeIter);
      }
      else if(is_flippable(halfedgeIter))
      {
          tri.flip_edge(halfedgeIter);
          suspects[halfedgeIter] = 1;
          suspects[halfedgeIter->opposite()] = 1;
          mark_potential_suspects(tri,halfedgeIter);
                            
      }
    }
  } 
  
	// Output the triangulation in OFF format to standard output.
	std::cout << "Triangulation in OFF format:\n";
	tri.output_off(std::cout);
  return 0;
}
