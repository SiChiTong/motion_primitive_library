/**
 * @file mp_base_util.h
 * @brief base class for motion planning
 *
 * Base classes for planning
 */

#ifndef MP_BASE_UTIL_H
#define MP_BASE_UTIL_H

#include <motion_primitive_library/planner/graph_search.h>
#include <motion_primitive_library/planner/env_base.h>


/**
 * @brief Motion planning base util class
 */
class MPBaseUtil
{
  public:
    ///Simple constructor
    MPBaseUtil();
    ///Check if it has been planned
    bool initialized();
    ///Get nodes on the optimal trajectory
    std::vector<Waypoint> getWs() const;
    ///Get optimal trajectory
    Trajectory getTraj() const;
    ///Get expanded collision free primitives
    std::vector<Primitive> getValidPrimitives() const;
    ///Get expanded primitives
    std::vector<Primitive> getAllPrimitives() const;
    ///Get points in open set
    vec_Vec3f getOpenSet() const;
    ///Get points in close set
    vec_Vec3f getCloseSet() const;
    ///Get points neither in open nor close set
    vec_Vec3f getNullSet() const;
    ///Get expanded points, for A* it should be the same as the close set
    vec_Vec3f getExpandedNodes() const;
    ///Get number of expanded nodes
    int getExpandedNum() const;
    /**
     * @brief Prune state space
     * @param time_step set the root of state space to be the waypoint on the best trajectory at best_child_[time_step]
     */
    void getSubStateSpace(int time_step);
    ///Check tree validation
    void checkValidation();

    ///Reset state space
    void reset();
    ///Set max vel in each axis
    void setLPAstar(bool use_lpastar);
    ///Set max vel in each axis
    void setVmax(decimal_t v);
    ///Set max acc in each axis
    void setAmax(decimal_t a);
    ///Set max jerk in each axis
    void setJmax(decimal_t j);
    ///Set max control in each axis
    void setUmax(decimal_t u);
    ///Set max time step to explore
    void setTmax(decimal_t t);
    ///Set prior trajectory
    void setPriorTrajectory(const Trajectory& traj);
    ///Set dt for each primitive
    void setDt(decimal_t dt);
    ///Set weight for cost in time
    void setW(decimal_t w);
    ///Set alpha in time offset
    void setAlpha(int alpha);
    ///Set greedy searching param 
    void setEpsilon(decimal_t eps);
    ///Set max number of expansion
    void setMaxNum(int num);
    ///Enable U through discretization 
    void setU(int n, bool use_3d);
    ///Set U 
    void setU(const vec_Vec3f& U);
    ///Set tolerance in geometric and dynamic spaces
    void setTol(decimal_t tol_dis, decimal_t tol_vel, decimal_t tol_acc = 0.0);
    /**
     * @brief Planning thread
     * @param start start waypoint
     * @param goal goal waypoint
     *
     * The start waypoint 
     * The goal waypoint is the center of the goal region, the planner cannot find the trajectory hit the excat goal state due to discretization;
     */
    bool plan(const Waypoint &start, const Waypoint &goal);

  protected:
    ///Env class
    std::shared_ptr<MPL::env_base> ENV_;
    ///Planner workspace
    std::shared_ptr<MPL::StateSpace> ss_ptr_;
    ///Intermediate nodes in optimal trajectory
    std::vector<Waypoint> ws_;
    ///Optimal trajectory
    Trajectory traj_;
    ///Greedy searching parameter
    decimal_t epsilon_ = 1.0;
    ///Maxmum number of expansion, -1 means no limitation
    int max_num_ = -1;
    ///Maxmum time horizon of expansion, 0 means no limitation
    double max_t_ = 0;
    ///Enable LPAstar for planning
    bool use_lpastar_ = false;


    ///Enabled to display debug message
    bool planner_verbose_;
};


#endif
