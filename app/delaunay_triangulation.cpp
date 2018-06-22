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

template <class T>
void lop()
{
    std::cout<< "You are here" << std::endl;
}

//find neighboring faces, return edges associated
void mark_potential_suspects(const Triangulation& tri, Triangulation::Halfedge_const_handle flippedEdge, Suspect_Map& suspects)
{
  std::cout << "faces in triangulation:\n";
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
        suspects[h->next()] = 0;
        suspects[h->next()->next()] = 0;
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
  lop<double>();
  //map to tag the suspects
  Suspect_Map suspects;
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
    suspects[halfedgeIter] = 0;
	}

    // for each edge
  while(!is_everythig_optimal(suspects))
  {
    for (auto halfedgeIter = tri.halfedges_begin(); halfedgeIter != tri.halfedges_end(); ++++halfedgeIter) {
      Triangulation::Vertex_const_handle a = halfedgeIter->vertex();
      Triangulation::Vertex_const_handle b =   
      halfedgeIter->opposite()->vertex();

      //if not preferred direction localy delauny,
      auto pa = halfedgeIter->opposite()->next()->vertex()->point();
      auto pb = halfedgeIter->vertex()->point();
      auto pc = halfedgeIter->opposite()->vertex()->point();
      auto pd = halfedgeIter->next()->vertex()->point();
      if(k.is_locally_pd_delaunay_edge(pa,pb,pc,pd,u,v))
      {
          tri.flip_edge(halfedgeIter);
          suspects[halfedgeIter] = 1;
          mark_potential_suspects(tri,halfedgeIter,suspects);
      }
      else
      {
          suspects[halfedgeIter] = 1;          
      }
    }
  } 
  

	// Output the triangulation in OFF format to standard output.
	std::cout << "Triangulation in OFF format:\n";
	tri.output_off(std::cout);
  return 0;
}