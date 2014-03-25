#include "rcpp_term_delaunay.h"

RCPP_MODULE(term_module) {
	class_<Del2TermType2D>( "Del2TermType2D" )
	.constructor()
    .field( "locBefore", &Del2TermType2D::locBefore, "documentation for locBefore" )
    .field( "locAfter", &Del2TermType2D::locAfter, "documentation for locAfter" )
    .property( "graph", &Del2TermType2D::get_struct, &Del2TermType2D::set_struct,
    "graph structure" )
    .property( "mode", &Del2TermType2D::get_mode, &Del2TermType2D::set_mode,
    "mode " )
	.property( "exprs", &Del2TermType2D::get_exprs, &Del2TermType2D::set_exprs,
    "exprs list" )
    .property( "cexprs", &Del2TermType2D::get_cexprs, &Del2TermType2D::set_cexprs,
    "common exprs list" )
    .property( "infos", &Del2TermType2D::get_infos, &Del2TermType2D::set_infos,
    "infos list" )
    .property( "params", &Del2TermType2D::get_params, &Del2TermType2D::set_params,
    "params list" )
    .method("set_point",&Del2TermType2D::set_current,"set point to insert")
    .method("get_point",&Del2TermType2D::get_current,"get point")
	.method("eval_first_expr",&Del2TermType2D::eval_first_expr,"eval first expr")
	;
	
	class_<Del2TermType3D>( "Del2TermType3D" )
	.constructor()
    .field( "locBefore", &Del2TermType3D::locBefore, "documentation for locBefore" )
    .field( "locAfter", &Del2TermType3D::locAfter, "documentation for locAfter" )
    .property( "graph", &Del2TermType3D::get_struct, &Del2TermType3D::set_struct,
    "graph structure" )
    .property( "mode", &Del2TermType3D::get_mode, &Del2TermType3D::set_mode,
    "mode " )
	.property( "exprs", &Del2TermType3D::get_exprs, &Del2TermType3D::set_exprs,
    "exprs list" )
    .property( "cexprs", &Del2TermType3D::get_cexprs, &Del2TermType3D::set_cexprs,
    "common exprs list" )
    .property( "infos", &Del2TermType3D::get_infos, &Del2TermType3D::set_infos,
    "infos list" )
    .property( "params", &Del2TermType3D::get_params, &Del2TermType3D::set_params,
    "params list" )
    .method("set_point",&Del2TermType3D::set_current,"set point to insert")
    .method("get_point",&Del2TermType3D::get_current,"get point")
    .method("eval_first_expr",&Del2TermType3D::eval_first_expr,"eval first expr")
	//.method("eval_exprs",&Del2EnergyType2D::eval_exprs_for,"eval exprs for a specified point")
	;
}
