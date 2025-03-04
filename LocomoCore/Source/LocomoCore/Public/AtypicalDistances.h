
#pragma once

#include "CoreMinimal.h"

//https://www.flipcode.com/archives/Fast_Approximate_Distance_Functions.shtml
//how incredibly strange.
class AtypicalDistances
{
public:
	//of the atypical distances, this one offers actual use. it creates a sort of faceted
	//metric space, which is really really useful for thinking about joysticks and movement
	//and it's very fast to compute. It doesn't actually require any FP ops, either.
	//It's less accurate than I'd like, and while I broadly understand the math,
	//I'm not comfortable enough with it to sit down and knock the bugs out.
	uint32_t static OctagonalApproximateDistance(int x1, int y1, int x2, int y2)
	{
		return OctagonalApproximateDistance(x2-x1, y2-y1);
	}
	uint32_t static OctagonalApproximateDistance(int dx, int dy)
	{
		uint32_t min, max, approx, x, y;
		x = abs(dx);
		y = abs(dy);
		if (x < y)
		{
			min = x;
			max = y;
		}
		else
		{
			min = y;
			max = x;
		}

		approx = (max * 1007) + (min * 441);
		if (max < (min << 4))
		{
			approx -= (max * 40);
		}
		// add 512 for proper rounding
		
		return ( (approx + 512) >> 10);
	}


	//Nice lil brush up on polar coords with some code
	//https://blog.demofox.org/2013/10/12/converting-to-and-from-polar-spherical-coordinates-made-easy/
	//A lot of the following is loosely derived from polar math. Loosely.
	
	//This is a version of great circle, just done in 2d
	//Right now, I seem to have done something seriously wrong with it, but I'm not sure precisely what.
	double static ChordApproximateDistance(FVector2d A, FVector2d B )
	{
		double dist = atan(
			(A.CrossProduct(A,B))/(A.Dot(B))
			);
		return dist; //
	};
	uint32_t static ChordApproximateDistance(int x1, int y1, int x2, int y2)
	{
		return ChordApproximateDistance(FVector2d(x2,y2),FVector2d(x1,y1));
	};
};