#ifndef RCPP_TERM_ALL_H
#define RCPP_TERM_ALL_H
#include "rcpp_spatstat_triangulation.h"
#include "rcpp_term_expr.h"

typedef TermType<ALL2,Delaunay2> All2TermType2D;
typedef TermType<ALL2,Delaunay3> All2TermType3D;

RCPP_EXPOSED_AS(All2TermType2D)
RCPP_EXPOSED_WRAP(All2TermType2D)
RCPP_EXPOSED_AS(All2TermType3D)
RCPP_EXPOSED_WRAP(All2TermType3D)
;

//specialization methods for All2TermType2D
template<>
List All2TermType2D::update_infos(HandleSet_Set set) {
  int i=0;
  List ret(set.size());
  for(
    HandleSet_Set::iterator eit = set.begin();
    eit != set.end();
    ++eit,++i
  ) {
    //std::cout << "all2 update i=" << i << "/" << set.size() << std::endl;
    Handle v0=*((*eit).begin()),v1=*((*eit).rbegin());
    Point_2 p0=v0->point(),p1=v1->point();
    List res;
    for(
      std::vector< std::string >::iterator cit = infos.begin();
      cit != infos.end();
      ++cit
    ) {
      std::string info=*cit;
      if(info=="x") {
        NumericVector x0(2);
        NumericVector x1(2);
        if(structure->is_infinite(v0)) {
            x0[0]=NA_REAL;
            x0[1]=NA_REAL;
          } else {
            x0[0]=p0.x();
            x0[1]=p0.y();
          }
          if(structure->is_infinite(v1)) {
            x1[0]=NA_REAL;
            x1[1]=NA_REAL;
          } else {
            x1[0]=p1.x();
            x1[1]=p1.y();
          }
          res["x"]=List::create(x0,x1);
      } else if (info=="l2") {
        if(structure->is_infinite(v0) || structure->is_infinite(v1))
          res["l2"]=NA_REAL;
        else
          res["l2"]=CGAL::squared_distance(p0,p1);
      } else if (info=="l") {
        if(structure->is_infinite(v0) || structure->is_infinite(v1))
          res["l"]=NA_REAL;
        else
          res["l"]=sqrt(CGAL::squared_distance(p0,p1));
          //std::cout << "l=" << sqrt(CGAL::squared_distance(p0,p1)) << std::endl;
      }  else if( info == "v") {
        res["v"]=List::create(v0->info(),v1->info());
      } else if( info == "a") {
        NumericVector a(2);
        double area=CGAL_Delaunay2_cell_area(structure,v0);
        a[0]=(area>0 ?  area : NA_REAL);
        area=CGAL_Delaunay2_cell_area(structure,v1);
        a[1]=(area>0 ? area : NA_REAL);
        res["a"]=a;
      }
      ret[i]=res;
    }

  }
  //std::cout << "all2 update return" << std::endl;
  return ret;
}


template <>
List All2TermType2D::update_infos(std::pair< HandleSet_Set,HandleSet_Set > sets) {
  return All2TermType2D::update_infos(sets.first);
}

template <>
List All2TermType2D::make_before_list() {
   Delaunay2_VertexSet_Set emptyEdges;
    return (locBefore = update_infos(emptyEdges));
}

template <>
List All2TermType2D::make_after_list() {
    HandleSet_Set incidentEdges;

    double r=(envir.exists("range") ? as<double>(envir.get("range")) : 1000000.0);
  //DEBUG: std::cout << "range=" << r << std::endl;
    incidentEdges=Delaunay2_All2_edges_at_range(structure,current_handle,r);
    //prepare the positive list
    //DEBUG: std::cout << "incidentEdges=" << incidentEdges.size() << std::endl;
    return (locAfter = update_infos(incidentEdges));
}

template <> template <>
void All2TermType2D::make_local_lists<INSERTION>() {
  //before INSERTION
  //All2TermType2D::make_before_list();

    //INSERTION
    apply_INSERTION();

    //after INSERTION
    All2TermType2D::make_after_list();
    //as before
    apply_DELETION();
}

template <> template <>
void All2TermType2D::make_local_lists<DELETION>() {
  //before DELETION

    All2TermType2D::make_after_list();
  //   //DELETION
  //   apply_DELETION();
  // //after DELETION

  //   All2TermType2D::make_before_list();
  //   //As before!
  //   apply_INSERTION();

}

template <> template<>
void All2TermType2D::set_current_<INSERTION>(NumericVector p) {
  //check that structure is set!
  //TODO
  //set the current point
  current=Point_2(p[0],p[1]);
  //create the lists of edges
  if(auto_make_list) make_local_lists<INSERTION>();
}

template <> template<>
void All2TermType2D::set_current_<DELETION>(NumericVector p) {
  double i=p[0];
  current_index=i;
  if(current_index <0 || current_index >= structure->number_of_vertices()) {
    //DEBUG: std::cout << "current_index=" << current_index ;
    //DEBUG: std::cout << " not in [0,"<<structure->number_of_vertices()-1<<"]" << std::endl;
    return;
  }
  //DEBUG: std::cout << "current_index=" << current_index <<std::endl;
  current_handle=Triangulation_vertex_at_pos<Delaunay2>(structure,current_index);

  current=current_handle->point();
  current_info=current_handle->info();
  //create the lists of edges
  if(auto_make_list) make_local_lists<DELETION>();
}

template <>
NumericVector All2TermType2D::get_current() {
  return NumericVector::create(current.x(),current.y());
}


//specialisation methods for All2TermType3D

template<>
List All2TermType3D::update_infos(HandleSet_Set set) {
  int i=0;
  List ret(set.size());
  for(
    HandleSet_Set::iterator eit = set.begin();
    eit != set.end();
    ++eit,++i
  ) {
    Handle v0=*((*eit).begin()),v1=*((*eit).rbegin());
    Point_3 p0=v0->point(),p1=v1->point();
    List res;
    for(
      std::vector< std::string >::iterator cit = infos.begin();
      cit != infos.end();
      ++cit
    ) {
      std::string info=*cit;
      if(info=="x") {
        NumericVector x0(3);
        NumericVector x1(3);
        // if(structure->is_infinite(v0)) {
       //      x0[0]=NA_REAL;
       //      x0[1]=NA_REAL;
       //      x0[2]=NA_REAL;
       //    } else {
            x0[0]=p0.x();
            x0[1]=p0.y();
            x0[2]=p0.z();
          // }
          // if(structure->is_infinite(v1)) {
          //   x1[0]=NA_REAL;
          //   x1[1]=NA_REAL;
          //   x1[2]=NA_REAL;
          // } else {
            x1[0]=p1.x();
            x1[1]=p1.y();
            x1[2]=p1.z();
          // }
          res["x"]=List::create(x0,x1);
      } else if (info=="l2") {
        //if(structure->is_infinite(v0) || structure->is_infinite(v1))
        //  res["l2"]=NA_REAL;
        //else
          res["l2"]=pow(p0.x()-p1.x(),2) + pow(p0.y()-p1.y(),2) + pow(p0.z()-p1.z(),2);

      } else if (info=="l") {
        // if(structure->is_infinite(v0) || structure->is_infinite(v1))
        //  res["l"]=NA_REAL;
        // else
          res["l"]=sqrt(pow(p0.x()-p1.x(),2) + pow(p0.y()-p1.y(),2) + pow(p0.z()-p1.z(),2));

      } else if( info == "v") {
        res["v"]=List::create(v0->info(),v1->info());
      }
      ret[i]=res;
    }

  }
  return ret;
}

template<>
List All2TermType3D::update_infos(std::pair< HandleSet_Set, HandleSet_Set> sets) {
  return All2TermType3D::update_infos(sets.first);
}


template <>
List All2TermType3D::make_before_list() {
   Delaunay3_VertexSet_Set emptyEdges;
    return (locBefore = update_infos(emptyEdges));
}

template <>
List All2TermType3D::make_after_list() {
    HandleSet_Set incidentEdges;
    double r=(envir.exists("range") ? as<double>(envir.get("range")) : 1000000.0);
    incidentEdges=Delaunay3_All2_edges_at_range(structure,current_handle,r);
    //prepare the positive list
    return (locAfter=update_infos(incidentEdges));
}

template <> template <>
void All2TermType3D::make_local_lists<INSERTION>() {
  //before INSERTION
  All2TermType3D::make_before_list();

    //INSERTION
    apply_INSERTION();

    //after INSERTION
    All2TermType3D::make_after_list();
    //as before
    apply_DELETION();
}

template <> template <>
void All2TermType3D::make_local_lists<DELETION>() {
  //before DELETION

    All2TermType3D::make_after_list();
    //DELETION
    apply_DELETION();
  //after DELETION

    All2TermType3D::make_before_list();
    //As before!
    apply_INSERTION();

}

template <> template<>
void All2TermType3D::set_current_<INSERTION>(NumericVector p) {
  //check that structure is set!
  //TODO
  //set the current point
  current=Point_3(p[0],p[1],p[2]);
  //create the lists of edges
  if(auto_make_list)  make_local_lists<INSERTION>();
}

template <> template<>
void All2TermType3D::set_current_<DELETION>(NumericVector p) {
  double i=p[0];
  current_index=i;
  if(current_index <0 || current_index >= structure->number_of_vertices()) return;
  //DEBUG: std::cout << "current_index=" << current_index <<std::endl;
  current_handle=Triangulation_vertex_at_pos<Delaunay3>(structure,current_index);
  current=current_handle->point();
  current_info=current_handle->info();
  //create the lists of edges
  if(auto_make_list) make_local_lists<DELETION>();
}

template <>
NumericVector All2TermType3D::get_current() {
  return NumericVector::create(current.x(),current.y(),current.z());
}

#endif //RCPP_TERM_ALL_H
