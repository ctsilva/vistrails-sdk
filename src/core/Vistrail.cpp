/*
 * Copyright (c) 2009-2011 VisTrails, Inc..
 *
 * This file is part of VisTrails SDK
 * (see https://github.com/vistrails/sdk).
 *
 * License: 3-clause BSD, see https://opensource.org/licenses/BSD-3-Clause
 */

#include "Vistrail.h"
#include "global_private.h"
#include "VersionTree.h"
#include "VersionTreeTraversal.h"
#include "Property.h"
#include <boost/unordered_set.hpp>
#include <boost/unordered_map.hpp>
#include <vector>

VT_BEGIN_NAMESPACE

typedef boost::unordered_map<VersionId,Version*> IdMap;

class Vistrail::PrivateImpl
{
public:
  PrivateImpl(): versionTree(new VersionTree) {}
  ~PrivateImpl() { delete this->versionTree; }
  VersionTree*  versionTree;
  IdMap         versionMap;
  PDict         annotations;
};

VT_DEFINE_PIMPL(Vistrail);

Vistrail::Vistrail(): self(new PrivateImpl())
{
}

VersionTree* Vistrail::versionTree()
{
  return this->self->versionTree;
}

void Vistrail::addVersion(Version *version)
{
  if (version->id() == VERSION_INVALID) {
    VersionId id = VersionId::gen();
    version->setId(id);
  }
  this->self->versionMap[version->id()] = version;
  this->self->versionTree->add(version->id(), version->parent());
}

Version* Vistrail::getVersion(const VersionId &id) const
{
  IdMap::iterator it = this->self->versionMap.find(id);
  if (it!=this->self->versionMap.end())
    return (*it).second;
  else
    return NULL;
}

PDict* Vistrail::annotations()
{
  return &this->self->annotations;
}

void Vistrail::getAllVersions(VersionList *list) const
{
  IdMap::iterator it = this->self->versionMap.begin();
  while (it!=this->self->versionMap.end()) {
    list->add((*it).second);
    it++;
  }
}

void Vistrail::getVersionPath(const VersionId &src,
                              const VersionId &dst,
                              VersionPath *path)
{
  boost::unordered_set<VersionId> visited;
  std::vector<VersionId> dstToCommon;
  VersionId id;
  VersionId common = VERSION_INVALID;
  VersionTree * tree = this->self->versionTree;

  if((this->getVersion(src) != NULL || src == VERSION_ROOT)
    && (this->getVersion(dst) != NULL || dst == VERSION_ROOT))
  {
    for (id = src; id != VERSION_INVALID; id = tree->getParent(id))
      visited.insert(id);

    for (id = dst; id != VERSION_INVALID; id = tree->getParent(id)) {
      if (visited.find(id) != visited.end()) {
        common = id;
        break;
      }
      dstToCommon.push_back(id);
    }

    path->clear();

    for (id = src; id != common; id = tree->getParent(id))
      path->add(this->getVersion(id));

    path->addRoot(this->getVersion(common));

    for (int i=(int)dstToCommon.size()-1; i>=0; --i)
      path->add(this->getVersion(dstToCommon[i]));
  }
}


void Vistrail::prune(const VersionId &id)
{
  VersionTreeTraversal traversal = VersionTreeTraversal(this->versionTree());
  traversal.traverse(id, TRAVERSE_BFS);
  while(!traversal.done())
  {
    this->self->versionMap.erase(traversal.current());
    traversal.next();
  }
  this->versionTree()->prune(id);
}

#define COST_TO_ROOT 0
#define COST_TO_CURRENT 1
#define COST_TO_ROOT_CHKPT 2
#define COST_TO_CUR_CHKPT 3

// void Vistrail::getActionPath(const VersionList &revVersionIds, 
//                              const VersionList &versionIds, 
//                              VersionId checkpointId,
//                              VersionPath *path
//                              ActionPath *path) {
//   if (checkpointId != VERSION_INVALID) {
//     path->set_needs_reset(true);
//     Version * version = this->getVersion(checkpointId);    
//     if (version != NULL) {
//       path->add(version->getCheckpointAction());
//     } else {
//       path->add(NULL);
//     }
//   }
//   for(std::vector<VersionId>::const_iterator it = revVersionIds.begin();
//       it != revVersionIds.end(); ++it) {
//     Version * version = this->getVersion(*it);
//     path->add(version->getInverseAction());
//   }
//   for(std::vector<VersionId>::const_iterator it = versionIds.begin();
//       it != versionIds.end(); ++it) {
//     Version * version = this->getVersion(*it);
//     path->add(version->getAction());
//   }
// }

// void Vistrail::getMaterializationPath(VersionId id, VersionId currentId,
// 				      ActionPath *path)
// {
//   typedef boost::unordered_set<VersionId> IdSet;
//   typedef std::vector<VersionId> VersionIdVector;
//   int i;
//   Version * version;
//   Version * commonParent = NULL;
//   Version * rootCheckpoint = NULL;
//   Version * curCheckpoint = NULL;
//   VersionIdVector pathToRoot, pathToCommonFromCur, pathToRootFromCommon,
//     pathToRootChkpt, pathToCommonFromCurChkpt, pathToCommon;
//   IdSet visitedToRoot;
//   std::vector<int> costs(4,-1);
//   int costToCommonFromCur = -1;
//   int costToCommon = -1;
//   int costToRootFromCommon = -1;
//   int costToCheckpointFromCur = -1;
//   int costToCommonFromCheckpoint = -1;
//   bool canInvertFromCurrent = true;
//   bool canInvertFromCurCheckpoint = false;

//   // find path to root, note nearest checkpoint
//   for (version=this->getVersion(id), i=0; 
//        version;
//        version=this->getVersion(version->getParent()), i++) {
//     if (rootCheckpoint == NULL && version->hasCheckpoint()) {
//       rootCheckpoint = version;
//       costs[COST_TO_ROOT_CHKPT] = i;
//       pathToRootChkpt = pathToRoot;
//     }
//     pathToRoot.push_back(version->getId());
//     visitedToRoot.insert(version->getId());
//   }
//   costs[COST_TO_ROOT] = i;

//   // find path from current
//   // need to check for inverses
//   i = 0;
//   for (version=this->getVersion(currentId);
//        version;
//        version=this->getVersion(version->getParent())) {
//     if (commonParent == NULL) {
//       if (visitedToRoot.find(version->getId()) != visitedToRoot.end()) {
// 	commonParent = version;
// 	costToCommonFromCur = i;
// 	if (curCheckpoint != NULL) {
// 	  costToCommonFromCheckpoint = i - costToCheckpointFromCur;
// 	}
//       } else {
// 	if (version->hasCheckpoint()) {
// 	  curCheckpoint = version;
// 	  canInvertFromCurCheckpoint = true;
// 	  costToCheckpointFromCur = i;
// 	  pathToCommonFromCurChkpt.clear();
// 	}
// 	if (!version->hasInverse()) {
// 	  canInvertFromCurCheckpoint = false;
// 	  canInvertFromCurrent = false;
// 	}
// 	pathToCommonFromCur.push_back(version->getId());
// 	pathToCommonFromCurChkpt.push_back(version->getId());
//       }
//     }
//     ++i;
//   }
//   costToRootFromCommon = i - costToCommonFromCur;
  
//   // determine length to commonParent and update costs to common
//   if (commonParent != NULL) {
//     // i = 0;
//     // for (VersionIdVector::const_iterator it=pathToRoot.begin(); 
//     // 	 it != pathToRoot.end();
//     // 	 ++it) { ++i; }
//     // costToCommon = i;
//     i = 0;
//     for (VersionIdVector::const_iterator it=pathToRoot.begin();
// 	 it != pathToRoot.end(); ++it) {
//       if ((*it) == commonParent->getId()) {
// 	break;
//       }
//       pathToCommon.push_back(*it);
//       ++i;
//     }
//     costToCommon = i;
//     // costToCommon = costs[COST_TO_ROOT] - costToRootFromCommon;
//     if (canInvertFromCurrent) {
//       costs[COST_TO_CURRENT] = costToCommonFromCur + costToCommon;
//       printf("@@@ COST TO CURRENT: %d\n", costs[COST_TO_CURRENT]);
//     }
//     if (canInvertFromCurCheckpoint) {
//       costs[COST_TO_CUR_CHKPT] = costToCommonFromCheckpoint + costToCommon;
//     }
//   }

//   // determine where the minimal valid cost is
//   for(std::vector<int>::const_iterator it=costs.begin(); it != costs.end();
//       ++it) {
//     printf("cost %d\n", *it);
//   }
//   int maxCost = *(std::max_element(costs.begin(), costs.end()));
//   std::replace(costs.begin(), costs.end(), -1, maxCost + 1);
//   size_t minIdx = std::min_element(costs.begin(), costs.end()) - costs.begin();
//   switch (minIdx) {
//   case COST_TO_ROOT:
//     printf("### ROOT\n");
//     std::reverse(pathToRoot.begin(), pathToRoot.end());
//     this->getActionPath(VersionIdVector(), pathToRoot, VERSION_ROOT, path);
//     // this->getPath(VERSION_ROOT, id, path);
//     break;
//   case COST_TO_CURRENT:
//     printf("### CURRENT\n");
//     std::reverse(pathToCommon.begin(), pathToCommon.end());
//     // this->getPath(currentId, id, path);
//     this->getActionPath(pathToCommonFromCur, pathToCommon, VERSION_INVALID, 
// 			path);
//     break;
//   case COST_TO_ROOT_CHKPT:
//     printf("### ROOT CHKPT\n");
//     std::reverse(pathToRootChkpt.begin(), pathToRootChkpt.end());
//     this->getActionPath(VersionIdVector(), pathToRootChkpt, 
// 			rootCheckpoint->getId(), path);
//     // this->getPath(rootCheckpoint->getId(), id, path);
//     break;
//   case COST_TO_CUR_CHKPT:
//     printf("### CUR CHKPT\n");
//     std::reverse(pathToCommon.begin(), pathToCommon.end());
//     this->getActionPath(pathToCommonFromCurChkpt, pathToCommon, 
// 			curCheckpoint->getId(), path);
//     // this->getPath(curCheckpoint->getId(), id, path);
//     break;
//   }
// }

VT_END_NAMESPACE
