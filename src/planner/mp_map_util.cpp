#include <motion_primitive_library/planner/mp_map_util.h>

using namespace MPL;

MPMapUtil::MPMapUtil(bool verbose) {
  planner_verbose_ = verbose;
  if(planner_verbose_)
    printf(ANSI_COLOR_CYAN "[MPPlanner] PLANNER VERBOSE ON\n" ANSI_COLOR_RESET);
}

void MPMapUtil::setMapUtil(std::shared_ptr<VoxelMapUtil>& map_util) {
  ENV_.reset(new MPL::env_map(map_util));
  map_util_ = map_util;
}

vec_Vec3f MPMapUtil::getLinkedNodes() const {
  lhm_.clear();
  vec_Vec3f linked_pts;
  for(const auto& it: ss_ptr_->hm_) {
    if(!it.second)
      continue;
    //check pred array
    for(unsigned int i = 0; i < it.second->pred_hashkey.size(); i++) {
      Key key = it.second->pred_hashkey[i];
      Primitive pr;
      ENV_->forward_action( ss_ptr_->hm_[key]->coord, it.second->pred_action_id[i], pr );
      double max_v = std::max(std::max(pr.max_vel(0), pr.max_vel(1)), pr.max_vel(2));
      int n = 1.0 * std::ceil(max_v * pr.t() / map_util_->getRes());
      int prev_id = -1;
      std::vector<Waypoint> ws = pr.sample(n);
      for(const auto& w: ws) {
        int id = map_util_->getIndex(map_util_->floatToInt(w.pos));
        if(id != prev_id) {
          linked_pts.push_back(map_util_->intToFloat(map_util_->floatToInt(w.pos)));
          lhm_[id].push_back(std::make_pair(it.second->hashkey, i));
          prev_id = id;
        }
      }
    }
  }

  return linked_pts;
}


std::vector<Primitive> MPMapUtil::updateBlockedNodes(const vec_Vec3i& blocked_pns) {
  std::vector<std::pair<Key, int>> blocked_nodes;
  for(const auto& it: blocked_pns) {
    int id = map_util_->getIndex(it);
    auto search = lhm_.find(id);
    if(search != lhm_.end()) {
      for(const auto& node: lhm_[id]) 
        blocked_nodes.push_back(node);
    }
  }

  return ss_ptr_->increaseCost(blocked_nodes, ENV_);
}


std::vector<Primitive> MPMapUtil::updateClearedNodes(const vec_Vec3i& cleared_pns) {
  std::vector<std::pair<Key, int>> cleared_nodes;
  for(const auto& it: cleared_pns) {
    int id = map_util_->getIndex(it);
    auto search = lhm_.find(id);
    if(search != lhm_.end()) {
      for(const auto& node: lhm_[id]) 
        cleared_nodes.push_back(node);
    }
  }
  
  return ss_ptr_->decreaseCost(cleared_nodes, ENV_);
}
