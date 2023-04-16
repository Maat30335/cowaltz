#include "bvh.h"
#include <algorithm>
#include <iostream>


Bounds3f BVHnode::WorldBounds() const{
    return bounds;
}

bool BVHnode::Intersect(const Ray &r, SurfaceInteraction *isect) const{
    if(!bounds.IntersectP(r, nullptr, nullptr)) return false;
    bool leftHit = left->Intersect(r, isect);
    bool rightHit = right->Intersect(r, isect);
    return leftHit || rightHit;
}

bool BVHnode::IntersectP(const Ray &r) const{
    if(!bounds.IntersectP(r, nullptr, nullptr)) return false;
    return (left->IntersectP(r) || right->IntersectP(r));
}


std::shared_ptr<Primitive> BVHnode::createBVH(const std::vector<std::shared_ptr<Primitive>> &prims){
    std::cout << "loading BVH..." << std::endl;

    std::vector<primInfo> info (prims.size());
    for(int i = 0; i < prims.size(); i++){
        info[i] = {i, prims[i]->WorldBounds()};
    }

    int count = 0;

    std::shared_ptr<Primitive> result = recursiveBuild(info, prims, 0, info.size(), &count);

    std::cout << "There are " << count << " prims at the bottom" << std::endl;

    return result;

}

std::shared_ptr<Primitive> BVHnode::recursiveBuild(std::vector<primInfo> &info, const std::vector<std::shared_ptr<Primitive>> &prims, int start, int end, int *totalPrims){
    
    Bounds3f centroidBounds;
    Bounds3f bounds;
    for(int i = start; i < end; i++){
        centroidBounds = Union(centroidBounds, info[i].centroid);
        bounds = Union(bounds, info[i].bounds);
    }
    int axis = centroidBounds.MaximumExtent();

    if(end - start <= 4){
        if(end - start == 1){
            *totalPrims += 1;
            return prims[info[start].primNum];
        }
        int mid = (start + end) / 2;
        std::nth_element(std::next(info.begin(), start), std::next(info.begin(), mid), 
                        std::next(info.begin(), end),
           [axis](const primInfo &a, const primInfo &b) { 
               return a.centroid[axis] < b.centroid[axis];
        });
        std::shared_ptr<BVHnode> node = std::make_shared<BVHnode>();
        node->left = recursiveBuild(info, prims, start, mid, totalPrims);
        node->right = recursiveBuild(info, prims, mid, end, totalPrims);
        node->bounds = bounds;
        return std::static_pointer_cast<Primitive>(node);

    }




    constexpr int nBuckets = 8;


    struct Bucket {
        int count = 0;
        Bounds3f bounds;
    };

    Bucket buckets[nBuckets];

    for(int i = start; i < end; i++){
        double p = (info[i].centroid[axis] - centroidBounds.pMin[axis]) / (centroidBounds.pMax[axis] - centroidBounds.pMin[axis]);
        int b = nBuckets * p;
        if (b == nBuckets) b = nBuckets - 1;
        buckets[b].count++;
        buckets[b].bounds = Union(buckets[b].bounds, info[i].bounds);
    }


    double cost[nBuckets - 1];

    for(int i = 0; i < nBuckets - 1; i++){
        Bounds3f b0, b1;
        int count0 = 0, count1 = 0;
        for(int j = 0; j <= i; j++){
            b0 = Union(b0, buckets[j].bounds);
            count0 += buckets[j].count;
        }
        for(int k = nBuckets - 1; k > i; k--){
            b1 = Union(b1, buckets[k].bounds);
            count1 += buckets[k].count;
            
        }
        cost[i] = .125 + (count0 * b0.SurfaceArea() + count1 * b1.SurfaceArea()) / bounds.SurfaceArea();
    }

    double minCost = cost[0];
    int minPos = 0;

    for(int i = 1; i < nBuckets - 1; i++){
        if(cost[i] < minCost){
            minPos = i;
            minCost = cost[i];
        }
    }

    if(minCost < (double)(end - start)){
        auto pmid = std::partition(std::next(info.begin(), start), std::next(info.begin(), end), 
                        [=] (const primInfo &pi){
                            double p = (pi.centroid[axis] - centroidBounds.pMin[axis]) / (centroidBounds.pMax[axis] - centroidBounds.pMin[axis]);
                            int b = nBuckets * p;
                            if (b == nBuckets) b = nBuckets - 1;
                            return b <= minPos;
                        });
        int mid = pmid - info.begin();

        std::shared_ptr<BVHnode> node = std::make_shared<BVHnode>();
        node->left = recursiveBuild(info, prims, start, mid, totalPrims);
        node->right = recursiveBuild(info, prims, mid, end, totalPrims);
        node->bounds = bounds;
        return std::static_pointer_cast<Primitive>(node);


    }else{
        std::shared_ptr<PrimitiveList> list = std::make_shared<PrimitiveList>();
        for(int i = start; i < end; i++){
            list->addPrim(prims[info[i].primNum]);
            *totalPrims += 1;
        }

        
        

        return std::static_pointer_cast<Primitive>(list);
    }
}
