/**
 * Content
 * Definition of the Network class
 *
 * @author: Can Yang
 * @version: 2017.11.11
 */

#ifndef FMM_NETWORK_HPP
#define FMM_NETWORK_HPP

#include "network/type.hpp"
#include "core/gps.hpp"

#include <ogrsf_frmts.h> // C++ API for GDAL
#include <iostream>
#include <math.h> // Calulating probability
#include <iomanip>
#include <algorithm> // Partial sort copy
#include <unordered_set> // Partial sort copy

// Data structures for Rtree
#include <boost/geometry/geometries/box.hpp>
#include <boost/geometry/index/rtree.hpp>
#include <boost/function_output_iterator.hpp>

namespace FMM
{
/**
 * Classes related with network and graph
 */
namespace NETWORK {
class Network {
 public:
  // Rtree data types
  typedef boost::geometry::model::box<Point> boost_box;
  typedef std::pair<boost_box, Edge *> Item;
  typedef boost::geometry::index::rtree<
      Item, boost::geometry::index::quadratic<16> > Rtree;
  /**
   *  Constructor of Network
   *
   *  @param filename: the path to a network file in ESRI shapefile format
   *  @param id_name: the name of the id field
   *  @param source_name: the name of the source field
   *  @param target_name: the name of the target field
   *
   */
  Network(const std::string &filename,
          const std::string &id_name = "id",
          const std::string &source_name = "source",
          const std::string &target_name = "target");   // Network constructor

  int get_node_count() const;

  int get_edge_count() const;

  const std::vector<Edge> &get_edges() const;

  EdgeID get_edge_id(EdgeIndex index) const;

  EdgeIndex get_edge_index(EdgeID id) const;

  NodeID get_node_id(NodeIndex index) const;

  NodeIndex get_node_index(NodeID id) const;

  Point get_node_geom_from_idx(NodeIndex index) const;

  /**
   *  Search for k nearest neighboring (KNN) candidates of a
   *  trajectory within a search radius
   *
   *  @param trajectory: input trajectory
   *  @param k: the number of candidates
   *  @param radius: the search radius
   *  @return a 2D vector of Candidates containing
   *  the candidates selected for each point in a trajectory
   *
   */
  Traj_Candidates search_tr_cs_knn(Trajectory &trajectory, std::size_t k,
                                   double radius) const;

  /**
   *  Search for k nearest neighboring (KNN) candidates of a
   *  linestring within a search radius
   */
  Traj_Candidates search_tr_cs_knn(const LineString &geom, std::size_t k,
                                   double radius) const;

  const LineString &get_edge_geom(int edge_id) const;
  /**
   * Extract the geometry of a complete path, whose two end segment will be
   * clipped according to the input trajectory
   */
  LineString complete_path_to_geometry(const LineString &traj,
                                       const C_Path &complete_path) const;

  const std::vector<Point> &get_vertex_points() const;

  const Point &get_vertex_point(NodeIndex u) const;

  LineString route2geometry(const std::vector<EdgeID> &path) const;

  LineString route2geometry(const std::vector<EdgeIndex> &path) const;

  static bool candidate_compare(const Candidate &a, const Candidate &b);
 private:
  /**
   * Concatenate a linestring segs to a linestring line, used in the
   * function complete_path_to_geometry
   *
   * @param line: linestring which will be updated
   * @param segs: segs that will be appended to line
   * @param offset: the number of points skipped in segs.
   */
  static void append_segs_to_line(LineString *line, const LineString &segs,
                                  int offset = 0);
  // Construct a Rtree using the vector of edges
  void build_rtree_index();
  int srid;   // Spatial reference id
  Rtree rtree;   // Network rtree structure
  std::vector<Edge> edges;   // all edges in the network
  NodeIDVec node_id_vec;
  unsigned int num_vertices;
  NodeIndexMap node_map;
  EdgeIndexMap edge_map;
  std::vector<Point> vertex_points;
}; // Network
} // NETWORK
} // FMM
#endif /* FMM_NETWORK_HPP */
