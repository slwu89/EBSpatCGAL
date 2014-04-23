#ifndef RCPP_SIM_H
#define RCPP_SIM_H
#include <Rcpp.h>
#include "rcpp_term_expr.h"

using namespace Rcpp ;

class SimGibbs { 

public:
    SimGibbs() {
        nb_runs=10000;
    }

    SimGibbs(List inter_,std::vector<double> left_,std::vector<double> right_) {
        set_interaction(inter_);
        set_domain(left_,right_);
        nb_runs=10000;
    }

    void set_interaction(List inter_) {
        inter=new Interaction(inter_);
    }

    // Indirectly set the mark expr of Interaction.
    // This is because Interaction is not set in R directly.
    // Same trick inside ListCache.
    void set_mark_expr(Language expr) {
        inter->set_mark_expr(expr);
    }

    void set_domain(Domain* domain_ ) {
        domain=domain_;
    }

    void set_domain(std::vector<double> left_,std::vector<double> right_) {
        domain=new Domain(left_,right_);
    }

    void set_single(double single_) {inter->single=single_;}

    double get_single() {return inter->single;}

    Domain* get_domain() {return domain;}

    //TODO: not adapted to a real domain
    void init_inside_number() {inside_number=inter->inside_number(domain);};

    void run() {
        init();
        init_inside_number();
        inter->sim_mode(true);
        for(int i=0;i<nb_runs;i++) run_once();
        inter->sim_mode(false);
    }

    int nb_runs;

    int nb_pts;

    double action_split;
    
    //maintain number of inside domain point
    int inside_number;
private:
    //STRUCT* structure;

    Domain* domain;

    Interaction* inter; //For Sim only the first expression is supposed to contain the local energy formula

    void init() {
        action_split=0.5;
        RNGScope scope;
    }

    TermMode propose_action() {
        double r = as<double>(runif(1,0,1));
        TermMode action=r<=action_split ? INSERTION : DELETION;
        //DEBUG: std::cout << "action: "<<action << std::endl;
        return action;
    };

    NumericVector pick_INSERTION() {return domain->pick();}

    NumericVector pick_DELETION() {
        return NumericVector::create(as<double>(runif(1,0,inside_number))); 
    };

    double value_INSERTION() {
        //self->area/(DOUBLE)nb_dv_total*EXP(-(self->local_energy)(func,poly))
        return (domain->get_size())/(inside_number)*exp(-(inter->local_energy()));
    } 

    double value_DELETION() {
        //Rmk: inter->local_energy returns insertion local energy only
        return (inside_number)/(domain->get_size())*exp(-(inter->local_energy()));
    } 

    void run_once() {

        double g = as<double>(runif(1,0,1)),val;
        if( inside_number == 0 ) {
            inter -> set_current(pick_INSERTION());
            if(g < (domain->get_size())*exp(inter->single)) {
                inter->apply_INSERTION();
                (inside_number)++;
            }
        } else if(propose_action() == INSERTION) {  
            inter-> set_current(pick_INSERTION());
            val=value_INSERTION();
            //DEBUG: std::cout << "nb=" << inside_number << " g=" << g << " value_INSERTION=" << val << std::endl;
            if(g < val ) (inside_number)++;
            else inter->apply_DELETION();
        } else {//else propose_action() == DELETION
            inter-> set_current(pick_DELETION());
            val=value_DELETION();
            //DEBUG: std::cout << "nb=" << inside_number << " g=" << g << " value_DELETION=" << val << std::endl;
            if(g <  val) (inside_number)--;
            else inter->apply_INSERTION();
        }

    };

};

#endif //RCPP_SIM_H