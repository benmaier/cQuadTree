//
//  QuadTree.h
//
//  Created by Ben F. Maier on 09.July.22.
//

#ifndef QuadTree_h
#define QuadTree_h

#include <Point.h>
#include <tuple>
#include <cmath>
#include <vector>
#include <string>
#include <sstream>

const int _NW = 0;
const int _NE = 1;
const int _SE = 2;
const int _SW = 3;

// string representations of the quadrants
const vector < string > _QUADS = {" (nw)", " (ne)", " (se)", " (sw)"};

using namespace std;

// reserve name to allow circular reference in class SubTrees
class QuadTree;

class SubTrees
{
  public:
    size_t occupied_trees;
    vector< QuadTree* > trees;

    SubTrees(){

        // initially, the node carries no subtrees
        occupied_trees = 0;
        
        // therefore, all quadrant pointers point to nothing
        trees.clear();
    }
    
    ~SubTrees(){
        // delete all subtrees that were created in runtime
        if (occupied_trees > 0){
            for(int i=3; i>=0; --i)
                if (trees[i]) delete trees[i];
            trees.clear();
        }
    }
    
    // add a new tree to one of the quadrants,
    // where iqad corresponds to the mapping above
    void add_tree(int iquad, QuadTree* tree){
        if (occupied_trees == 0){
            for(int i=0; i<4; ++i)
                trees.push_back(NULL);
        }
        occupied_trees += 1;
        trees[iquad] = tree;
    }

    QuadTree* get_subtree(int iquad){
        if (iquad < 0 || iquad > 3)
            throw range_error("The requested quadrant id was out of range [0,3].");
        if (occupied_trees==0)
            return NULL;
        else
            return trees[iquad];
    }
};

// this class takes care of the geometry of a tree node 
class Extent
{
  private:
    Point botLeft; // a vector containing the position of a box's lower left corner
    Point topRight; // a vector containing the position of a box's upper right corner
    Point vec; // a vector pointing from the lower left towards the upper right
    double w; // width of the box
    double h; // height of the box
    
  public:

    // initiate this empty, zero dimension
    Extent(){
        botLeft = Point(0.f,0.f);
        topRight = Point(0.f,0.f);
        vec = topRight - botLeft;
        w = vec.x;
        h = vec.y;
    }

    // initiate this from bottom-left and top-right vectors
    Extent(const Point &_botLeft,
           const Point &_topRight
           )
    {
        botLeft = _botLeft;
        topRight = _topRight;
        vec = topRight - botLeft;
        w = vec.x;
        h = vec.y;
    }
    
    // initiate this from bottom-left, width and height
    Extent(const Point &_botLeft,
           const double &_w,
           const double &_h
           )
    {
        botLeft = _botLeft;
        vec = Point(_w,_h);
        topRight = botLeft + vec;
        w = _w;
        h = _h;
    }

    // initiate this from bottom-left, width, and height, but the bottom-left coordinates are given explicitly
    Extent(const double &_x_bottom_left,
           const double &_y_bottom_left,
           const double &_w,
           const double &_h
           )
    {
        botLeft = Point(_x_bottom_left, _y_bottom_left);
        vec = Point(_w,_h);
        topRight = botLeft + vec;
        w = _w;
        h = _h;
    }

    // initiate from another box (copy attributes)
    Extent(const Extent &other){
        botLeft = other.botLeft;
        topRight = other.topRight;
        vec = other.vec;
        w = other.w;
        h = other.h;
    }
    
    // initiate from a list of 2d positions
    Extent(const vector <Point> &positions){
        
        if (positions.empty())
            return;
        
        // find the respective x and y min and max
        double minX, maxX, minY, maxY;
        bool noPointSet = true;
        for(auto const &pos: positions){
            if (noPointSet){
                minX = pos.x;
                maxX = pos.x;
                minY = pos.y;
                maxY = pos.y;
                noPointSet = false;
            } else {
                if (pos.x < minX)
                    minX = pos.x;
                if (pos.x > maxX)
                    maxX = pos.x;
                if (pos.y < minY)
                    minY = pos.y;
                if (pos.y > maxY)
                    maxY = pos.y;
            }
        }
        
        // set the extent attributes
        botLeft = Point(minX,minY);
        topRight = Point(maxX,maxY);
        vec = topRight - botLeft;
        w = vec.x;
        h = vec.y;
    }
    
    // return a new geometry referring to a box that corresponds to one of the quadrants,
    // accessed by integer id
    Extent get_quadrant(const int &q){
        if (q==_NW){
            return get_NW();
        } else if (q==_NE) {
            return get_NE();
        } else if (q==_SE) {
            return get_SE();
        } else if (q==_SW) {
            return get_SW();
        } else {
            throw range_error("Invalid quadrant id was passed (can only be 0, 1, 2, or 3).");
        }
    }
    
    // return the north-western quadrant
    Extent get_NW(){
        Point base = botLeft + Point(0.f,vec.y/2);
        return Extent(base, base + vec/2);
    }
    
    // return the north-eastern quadrant
    Extent get_NE(){
        Point base = botLeft + vec/2;
        return Extent(base, base + vec/2);
    }
    
    // return the south-eastern quadrant
    Extent get_SE(){
        Point base = botLeft + Point(vec.x/2,0.f);
        return Extent(base, base + vec/2);
    }
    
    // return the south-western quadrant
    Extent get_SW(){
        return Extent(botLeft, botLeft + vec/2);
    }
    
    // return the minimum x-coordinate of this box
    double left(){
        return botLeft.x;
    }
    
    // return the minimum y-coordinate of this box
    double bottom(){
        return botLeft.y;
    }

    // return the maximum x-coordinate of this box
    double right(){
        return topRight.x;
    }
    
    // return the maximum y-coordinate of this box
    double top(){
        return topRight.y;
    }
    
    // check whether a point lies within a box
    bool contains(const Point &pos){
        return ( pos.x >= left() &&
                 pos.x <= right() &&
                 pos.y <= top() &&
                 pos.y >= bottom()
                );
    }
    
    // Returns the integer id of the quadrant, 
    // this position would lie in.
    // Returns -1 if the position does not
    // lie within the extent of this box
    int quad_to_insert_to(const Point &pos){
        if (contains(pos)){
            // left space
            if (pos.x < right()-w/2) {
                // bottom space
                if (pos.y < top()-h/2){
                    return _SW;
                } else {
                    return _NW;
                }
            } else{
                // bottom space
                if (pos.y < top()-h/2){
                    return _SE;
                } else {
                    return _NE;
                }
            }
        }
        else{
            return -1;
        }
    }

    // returns width of this box
    double width(){
        return w;
    }

    // returns height of this box
    double height(){
        return h;
    }
    
    // returns the vector that points from the lower left to the upper right of the box
    Point get_vec(){
        return vec;
    }
    
    // returns the vector that contains the coordinates of the box's lower left
    Point get_bottom_left(){
        return botLeft;
    }

    // returns the vector that contains the coordinates of the box's upper right
    Point get_top_right(){
        return topRight;
    }
    
    // returns the vector that contains the coordinates of the box's upper left
    Point get_top_left(){
        return Point(botLeft.x, topRight.y);
    }

    string tostr() {
        ostringstream ss;
        ss << "Extent(left=" << left() 
           << ",bottom=" << bottom() 
           << ",width=" << w 
           << ",height=" << h
           << ")";

        return ss.str();
    }
};


// A tree root that contains positions and subtrees
class QuadTree
{
    
  private:

    // insert data into this node
    void _update_data(Point &pos, double mass){

        // to compute center of mass and total mass
        total_mass_position += mass * pos;
        total_mass += mass;
        center_of_mass = total_mass_position/total_mass;

        // number of points that lie within this tree (box) increases by one
        number_of_contained_points++;
    }

  public:

    Point this_pos = Point(nan(""), nan("")); // a pointer to the vector of the mass point that this tree carries.
    int this_id = -1;         // integer id of the data point in this node
    double this_mass = 0.f;    // the mass of the data point in this node
    double total_mass = 0.f;   // total mass of all data points that are contained in this node
                              // xor in all subtrees of this node
    Point total_mass_position = Point(0.f, 0.f); // total mass-position of all data points that are contained
                                           // in this node xor all subtrees of this node
    Point center_of_mass = Point(0.f, 0.f);      // center of mass of all data points in this node
    size_t number_of_contained_points = 0; // the total number of all data points that are contained
                                           // in this node xor in all its subtrees
    int current_data_quadrant = -1; // the quadrant the current data lies in
    Extent geom;                    // the geometry of the box of this node
    SubTrees subtrees;              // the subtrees of this node
    QuadTree* parent = NULL;   // the parent of this node (if root, parent is NULL)

    QuadTree(){
    };
    
    // create a root node that corresponds to a box and might have
    // been created from a parent node
    QuadTree(const Extent &_geom,
                  QuadTree* _parent = NULL
                  )
    {
        parent = _parent;
        geom = _geom;
    };
    
    // recursively create a whole tree from a list of positions,
    // masses will be set to m = 1 for every data point
    QuadTree(vector < Point > & positions,
             bool const &force_square=true
            )
    {
        geom = Extent(positions);
        if (force_square)
        {
            double max_dim = max(geom.width(), geom.height());
            geom = Extent(geom.left(), geom.bottom(), max_dim, max_dim);
        }

        insert_positions(positions);
    }

    QuadTree(vector < pair < double, double > > & position_pairs,
                  bool const &force_square=true
                  )
    {

        vector < Point > positions;

        for(auto const &pos: position_pairs)
            positions.push_back(Point(pos.first, pos.second));

        geom = Extent(positions);
        if (force_square)
        {
            double max_dim = max(geom.width(), geom.height());
            geom = Extent(geom.left(), geom.bottom(), max_dim, max_dim);
        }

        insert_positions(positions);
    }

    QuadTree(vector < pair < double, double > > & position_pairs,
                  vector < double > & masses,
                  bool const &force_square=true
                  )
    {

        vector < Point > positions;

        for(auto const &pos: position_pairs)
            positions.push_back(Point(pos.first, pos.second));

        geom = Extent(positions);
        if (force_square)
        {
            double max_dim = max(geom.width(), geom.height());
            geom = Extent(geom.left(), geom.bottom(), max_dim, max_dim);
        }

        insert_positions_and_masses(positions, masses);
    }

    // recursively create a whole tree from a list of positions and masses
    QuadTree(vector < Point > & positions,
             vector < double > & masses,
             bool const &force_square=true
    ){
        geom = Extent(positions);
        if (force_square)
        {
            double max_dim = max(geom.width(), geom.height());
            geom = Extent(geom.left(), geom.bottom(), max_dim, max_dim);
        }

        insert_positions_and_masses(positions, masses);
    }

    // insert a data point into the tree, including a mass and an
    // integer id of the data point (to reference the data point later)
    void insert(Point &new_pos, double mass = 1.0f, int id = -1){
        
        // find the quadrant of this box that the data point would be inserted to
        int candidate_quad = geom.quad_to_insert_to(new_pos);
        if (candidate_quad < 0) return; // if the candidate is -1, the point lies outside the box
        
        // if this tree node carries no data and no subtrees (i.e. is empty),
        // put the position and the mass inside this node and return
        if (is_empty()){
            this_pos = new_pos;
            current_data_quadrant = candidate_quad;
            this_mass = mass;
            _update_data(new_pos, mass);
            this_id = id;
            return;
        }
        
        // if this tree node carries no data but has subtrees (i.e. is an internal node of the tree),
        // find the subtree/quadrant this position would lie in and insert it in there
        if (is_internal_node()){
            
            QuadTree* tree_to_insert_to = subtrees.get_subtree(candidate_quad);
            
            // if the candidate tree/quadrant is empty, create a new tree in this quadrant
            if (tree_to_insert_to == NULL){
                tree_to_insert_to = new QuadTree(geom.get_quadrant(candidate_quad), this);
                subtrees.add_tree(candidate_quad, tree_to_insert_to);
            }

            // insert the data into either (a) this new leaf node or (b) the already existing tree
            tree_to_insert_to->insert(new_pos,mass,id);
            _update_data(new_pos, mass);

            return;
        }

        // if this tree node carries data and has no subtrees (i.e. is a leaf of the tree),
        // create a new tree in the quadrant of the old data, insert the old data into the subtree,
        // reset all data pointers of this former leaf node, then start the procedure
        // to insert the new data into this tree again
        if (is_leaf()) {
            
            // create subtree to which the new data will be inserted and insert new data
            QuadTree* new_tree = new QuadTree(geom.get_quadrant(current_data_quadrant), this);
            subtrees.add_tree(current_data_quadrant,new_tree);
            new_tree->insert(this_pos,this_mass,this_id);

            // reset the data pointers of this former leaf node that just became an internal node
            this_mass = 0.f;
            this_pos = Point(nan(""),nan(""));
            this_id = -1;
            current_data_quadrant = -1;
            
            // insert the new data into this tree, which is now an internal node 
            insert(new_pos, mass, id);
        }
    }

    void insert_positions(vector < Point > & positions){
        int i = 0;
        for(auto &pos: positions){
            insert(pos,1.0,i);
            ++i;
        }
    }

    void insert_positions_and_masses(
                  vector < Point > & positions,
                  vector < double > & masses
                  )
    {
        // check that every point has a mass
        if (masses.size() != positions.size())
            throw length_error("masses and positions must be of equal length");
        
        auto mass = masses.begin();
        int i = 0;
        for(auto &pos: positions){
            insert(pos, *mass, i);
            ++mass;
            ++i;
        }
    }

    bool is_leaf(){
        return ((!this_pos.is_null()) && subtrees.occupied_trees == 0);
    }

    bool is_internal_node(){
        return (this_pos.is_null() && subtrees.occupied_trees > 0);
    }

    bool is_empty(){
        return (this_pos.is_null() && subtrees.occupied_trees == 0);
    }

    void compute_force(
                 const Point &pos,
                 Point &force,
                 double theta = 0.5,
                 QuadTree* tree = NULL
            )
    {

        if (tree == NULL)
            tree = this;

        if (tree->is_leaf())
        {
            Point d = (tree->this_pos) - pos;
            double norm2 = d.length2();
            if (norm2 > 0)
                force += (tree->total_mass) * d/pow(norm2,1.5);
        }
        else
        {
            Point _r = tree->center_of_mass;
            Point d = (_r) - pos;
            double s2 = tree->geom.width() * tree->geom.height(); // geometric mean of box dimensions
            double norm2 = d.length2();
            if ((s2/norm2) < theta*theta)
                force += (tree->total_mass) * d/pow(norm2,1.5);
            else
                for(auto &subtree: tree->subtrees.trees){
                    if (subtree != NULL){
                        compute_force(pos, force, theta, subtree);
                    }
                }
        }
    }

    pair < double, double > compute_force_on_pair(
                 const pair < double, double > &pos,
                 double theta = 0.5
             )
    {

        Point force;
        compute_force(Point(pos.first, pos.second),
                      force,
                      theta,
                      this
                      );
        return make_pair(force.x, force.y);
    }


    void get_distances_to(
                 const Point &pos,
                 vector < pair < double, size_t > > &distances,
                 const double &theta = 0.2,
                 const bool &ignore_zero_distance = true,
                 QuadTree* tree = NULL
            )
    {
        if (tree == NULL)
            tree = this;
        if (tree->is_leaf())
        {
            Point d = (tree->this_pos) - pos;
            double norm2 = d.length2();
            if ((norm2 > 0) || (!ignore_zero_distance))
                distances.push_back(make_pair(sqrt(norm2), 1));
        }
        else
        {
            Point _r = tree->center_of_mass;
            Point d = (_r) - pos;
            double s2 = tree->geom.width() * tree->geom.height(); // geometric mean of box dimensions
            double norm2 = d.length2();
            if ((s2/norm2) < theta*theta)
                distances.push_back(make_pair(sqrt(norm2), (tree->number_of_contained_points)));
            else
                for(auto &subtree: tree->subtrees.trees){
                    if (subtree != NULL){
                        get_distances_to(pos, distances, theta, 
                                         ignore_zero_distance,
                                         subtree);
                    }
                }
        }
    }

    vector < pair < double, size_t > > get_distances_to_pair(
                 const pair < double, double > &pos,
                 const double &theta = 0.2,
                 const bool &ignore_zero_distance = true,
                 QuadTree* tree = NULL
            )
    {
        vector < pair < double, size_t > > distances;
        get_distances_to(Point(pos.first, pos.second), distances, theta, ignore_zero_distance, tree);
        return distances;
    }

    vector < pair < double, size_t > > get_distances_to_pairs(
                 const vector < pair < double, double > > &positions,
                 const double &theta = 0.2,
                 const bool &ignore_zero_distance = true,
                 QuadTree* tree = NULL
            )
    {
        vector < pair < double, size_t > > distances;
        for(auto const &pos: positions)
            get_distances_to(Point(pos.first, pos.second), distances, theta, ignore_zero_distance, tree);
        return distances;
    }

    vector < pair < double, size_t > > _get_pairwise_distances(
                 const double &theta = 0.2,
                 const bool &ignore_zero_distance = true
            ){
        return get_pairwise_distances(theta, ignore_zero_distance);

    }

    vector < pair < double, size_t > > get_pairwise_distances(
                 const double &theta = 0.2,
                 const bool &ignore_zero_distance = true,
                 vector < pair < double, size_t > >* distances = NULL,
                 QuadTree* node = NULL,
                 QuadTree* root = NULL
            )
    {
        
        vector < pair < double, size_t > > _distances;
        if (distances == NULL)
            distances = &_distances;
        if (node == NULL)
            node = this;
        if (root == NULL)
            root = this;

        if (node->is_leaf()){
            get_distances_to( (node->this_pos),
                              *distances,
                              theta,
                              ignore_zero_distance,
                              root
                              );
        }
        else
        {
            for(auto &subtree: node->subtrees.trees){
                if (subtree != NULL)
                    get_pairwise_distances(
                                theta,
                                ignore_zero_distance,
                                distances,
                                subtree,
                                root
                            );
            }
        }

        return (*distances);
    }

    // recursively construct a string stream representation of the tree
    void get_tree_str(
                      ostringstream &ss,
                      QuadTree* node = NULL,
                      string indent = "",
                      string quad = ""
                   ){

        if (node == NULL)
            node = this;
        
        ss << indent << "+-" << quad << " ";

        if (node->is_leaf()){
            ss << node->this_id  << " (" << (node->this_pos) << ")" << endl;
        }
        if (node->is_internal_node()){
            ss << "CM = " << node->center_of_mass << "; " << "M = " << node->total_mass << "; "
               << "n = " << node->number_of_contained_points << endl;

            int i = 0;
            for(auto &subtree: node->subtrees.trees){
                if (subtree != NULL)
                    get_tree_str(ss,subtree,indent + "| ",_QUADS[i]);
                ++i;
            }
        }
    }

    vector < QuadTree* > get_subtrees(){
        vector < QuadTree* > _sbtrs;
        for(int i=0; i<4; ++i){
            QuadTree* this_sub = subtrees.get_subtree(i);
            if (this_sub != NULL)
                _sbtrs.push_back(this_sub);
        }
        return _sbtrs;
    }

    QuadTree* get_subtree(int i){
        return subtrees.get_subtree(i);
    }

    string str() {
      ostringstream ss;
      get_tree_str(ss);
      return ss.str();
    }

    string tostr() {
      ostringstream ss;
      ss << "QuadTree(" << endl;
      ss << "    geom=" << geom.tostr() << "," << endl;
      ss << "    current_data_quadrant=" << current_data_quadrant << "," << endl;
      if (is_leaf()) {
          ss << "    is_leaf=True," << endl;
          if (!this_pos.is_null())
          {
              ss << "    this_pos=" << this_pos.tostr() << "," << endl;
              ss << "    this_mass=" << this_mass << endl;
          }
      } else {
          ss << "    is_leaf=False," << endl;
          ss << "    number_of_contained_points=" << number_of_contained_points << "," << endl;
          ss << "    center_of_mass=" << center_of_mass.tostr() << "," << endl;
          ss << "    total_mass=" << total_mass << "," << endl ;
          ss << "    total_mass_position=" << total_mass_position.tostr() << "," << endl;
          ss << "    number_of_occupied_subtrees=" << subtrees.occupied_trees << endl;
      }
      ss << ")";
      return ss.str();
    }
    
};


#endif /* QuadTree_h */

