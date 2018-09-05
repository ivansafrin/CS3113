#include "SatCollision.h"
#include <math.h>

bool TestSATSeparationForEdge(float edgeX, float edgeY, const std::vector<std::pair<float,float>> &points1, const std::vector<std::pair<float,float>> &points2, std::pair<float,float> &penetration) {
	float normalX = -edgeY;
	float normalY = edgeX;
	float len = sqrtf(normalX*normalX + normalY*normalY);
	normalX /= len;
	normalY /= len;
	
	std::vector<float> e1Projected;
	std::vector<float> e2Projected;
	
	for(int i=0; i < points1.size(); i++) {
		e1Projected.push_back(points1[i].first * normalX + points1[i].second * normalY);
	}
	for(int i=0; i < points2.size(); i++) {
		e2Projected.push_back(points2[i].first * normalX + points2[i].second * normalY);
	}
	
	std::sort(e1Projected.begin(), e1Projected.end());
	std::sort(e2Projected.begin(), e2Projected.end());
	
	float e1Min = e1Projected[0];
	float e1Max = e1Projected[e1Projected.size()-1];
	float e2Min = e2Projected[0];
	float e2Max = e2Projected[e2Projected.size()-1];
	
	float e1Width = fabs(e1Max-e1Min);
	float e2Width = fabs(e2Max-e2Min);
	float e1Center = e1Min + (e1Width/2.0);
	float e2Center = e2Min + (e2Width/2.0);
	float dist = fabs(e1Center-e2Center);
	float p = dist - ((e1Width+e2Width)/2.0);
	
	if(p >= 0) {
		return false;
	}
	
	float penetrationMin1 = e1Max - e2Min;
	float penetrationMin2 = e2Max - e1Min;
	
	float penetrationAmount = penetrationMin1;
	if(penetrationMin2 < penetrationAmount) {
		penetrationAmount = penetrationMin2;
	}
	
	penetration.first = normalX * penetrationAmount;
	penetration.second = normalY * penetrationAmount;
	
	return true;
}

bool PenetrationSort(const std::pair<float,float> &p1, const std::pair<float,float> &p2) {
	return sqrtf(p1.first*p1.first + p1.second*p1.second) < sqrtf(p2.first*p2.first + p2.second*p2.second);
}

bool CheckSATCollision(const std::vector<std::pair<float,float>> &e1Points, const std::vector<std::pair<float,float>> &e2Points, std::pair<float,float> &penetration) {
	std::vector<std::pair<float,float>> penetrations;
	for(int i=0; i < e1Points.size(); i++) {
		float edgeX, edgeY;
		
		if(i == e1Points.size()-1) {
			edgeX = e1Points[0].first - e1Points[i].first;
			edgeY = e1Points[0].second - e1Points[i].second;
		} else {
			edgeX = e1Points[i+1].first - e1Points[i].first;
			edgeY = e1Points[i+1].second - e1Points[i].second;
		}
		std::pair<float,float> penetration;
		bool result = TestSATSeparationForEdge(edgeX, edgeY, e1Points, e2Points, penetration);
		if(!result) {
			return false;
		}
		penetrations.push_back(penetration);
	}
	for(int i=0; i < e2Points.size(); i++) {
		float edgeX, edgeY;
		
		if(i == e2Points.size()-1) {
			edgeX = e2Points[0].first - e2Points[i].first;
			edgeY = e2Points[0].second - e2Points[i].second;
		} else {
			edgeX = e2Points[i+1].first - e2Points[i].first;
			edgeY = e2Points[i+1].second - e2Points[i].second;
		}
		std::pair<float,float> penetration;
		bool result = TestSATSeparationForEdge(edgeX, edgeY, e1Points, e2Points, penetration);
		
		if(!result) {
			return false;
		}
		penetrations.push_back(penetration);
	}
	
	std::sort(penetrations.begin(), penetrations.end(), PenetrationSort);
	penetration = penetrations[0];
	
	std::pair<float,float> e1Center;
	for(int i=0; i < e1Points.size(); i++) {
		e1Center.first += e1Points[i].first;
		e1Center.second += e1Points[i].second;
	}
	e1Center.first /= (float)e1Points.size();
	e1Center.second /= (float)e1Points.size();
	
	std::pair<float,float> e2Center;
	for(int i=0; i < e2Points.size(); i++) {
		e2Center.first += e2Points[i].first;
		e2Center.second += e2Points[i].second;
	}
	e2Center.first /= (float)e2Points.size();
	e2Center.second /= (float)e2Points.size();

	std::pair<float,float> ba;
	ba.first = e1Center.first - e2Center.first;
	ba.second = e1Center.second - e2Center.second;
	
	if( (penetration.first * ba.first) + (penetration.second * ba.second) < 0.0f) {
		penetration.first *= -1.0f;
		penetration.second *= -1.0f;
	}
	
	return true;
}
