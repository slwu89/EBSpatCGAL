#ifndef RCPP_DOM_H
#define RCPP_DOM_H
#include <Rcpp.h>
#include "rcpp_term_expr.h"

using namespace Rcpp ;

class Domain {
public: 
    // TODO : completer en spécifiant 4 types de domaines : 
    // Rectangulaires, Couronnes rectangulaires, Ellipsoidales et Couronnes ellipsoidales.
    // left,right délimitent contour extérieur quand left_in, right_in délimitent le contour intérieur
    // introduire mode= 1 (Ellipse) ou 0 (Rectangulaire)
    // Créer les méthodes pour:
    // 0) tester l'appartenance au domaine
    // 1) initialiser la liste des vertex_handle dans domaine 
    // 2) maintenir cette liste lors de suppression ou insertion de point
    // 3) générer aléatoire point à insérer ou à supprimer

    Domain() {}; //needed by rcpp_delaunay_module.cpp
    Domain(std::vector<double> left_,std::vector<double> right_) {//Rectangle first
        left=left_;right=right_;
        dim=left.size();
        grid_delta.reserve(3);
        //DEBUG: std::cout << "dim="<< dim << std::endl;
        set_size();
    };

    NumericVector pick() {
        std::vector<double> newPt(dim);
        for(int i=0;i<dim;i++) newPt[i]=as<double>(runif(1,left[i],right[i]));
        return NumericVector(newPt.begin(),newPt.end());
    };

    //for rcpp
    bool contains_(std::vector<double> coords) {
        for(int i=0;i<dim;i++) if ((left[i] > coords[i]) || (coords[i] > right[i])) return false;
        return true;
    }

    //for use in inside_number and inside_indexes (see rcpp_term_expr.h) 
    bool contains(double x,double y) {
        return (left[0] <= x) &&  (x <= right[0]) && (left[1] <= y) &&  (y <= right[1]);
    }

    bool contains(double x,double y,double z) {
        return (left[0] <= x) &&  (x <= right[0]) && (left[1] <= y) &&  (y <= right[1]) && (left[2] <= z) &&  (z <= right[2]);
    }


    double get_size() {return size;} //area or volume

    int get_dim() {return dim;}

    //used in gnz_cache
    void set_grid(std::vector<int> grid_size_) {
        grid_size=grid_size_;
        grid_length=1;
        for(int i=0;i<dim;i++) {
            grid_length *= grid_size[i];
            grid_delta[i] = (right[i]-left[i])/grid_size[i];
            //std::cout << "grid_delta[" << i <<"]" << grid_delta[i] << std::endl;
        }
        if(dim==2) grid_delta[2]= 1.0;
    }

    //used in gnz_cache for the grid points
    double get_left(int i) {return ( i>=0 && i<dim ? left[i] : 0);}

    double get_right(int i) {return ( i>=0 && i<dim ? right[i] : 0.5);}

    std::vector<double> left,right; //in some axe
    //std::vector<double> in_left,in_right; //in some axe

    std::vector<double> grid_delta;
    std::vector<int> grid_size;
    int grid_length;


protected:

    int dim;

    double size;

    void set_size() {
        size=1;
        for(int i=0;i<dim;i++) size *=right[i]-left[i];
    }

    // void update_inside_number() {
    //     for(HANDLE::iterator hit=structure->begin_vertices())
    // }
};
#endif //RCPP_DOM_H