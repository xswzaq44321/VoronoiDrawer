#include "sweepline.h"

using namespace voronoiMap;

Parabola::Parabola(voronoiMap::Point* focus, voronoiMap::Polygon* poly):
	event(nullptr),
	poly(poly)
{
	this->focus->x = focus->x;
	this->focus->y = focus->y;
}

Parabola::~Parabola()
{

}

Event::Event(voronoiMap::Polygon* rel)
{
	relevants.push_back(new Parabola(rel->focus, rel));
	this->x = rel->focus->x;
	isCircle = false;
}

Event::Event(Parabola *r1, Parabola *r2, Parabola *r3)
{
	relevants.push_back(r1);
	relevants.push_back(r2);
	relevants.push_back(r3);
	double X[3][3];
	double Y[3][3];
	double D[3][3];
	X[0][0] = (pow(r1->focus->x, 2) + pow(r1->focus->y, 2));
	X[1][0] = r1->focus->y;
	X[2][0] = 1;
	Y[0][0] = r1->focus->x;
	Y[1][0] = (pow(r1->focus->x, 2) + pow(r1->focus->y, 2));
	Y[2][0] = 1;
	D[0][0] = r1->focus->x;
	D[1][0] = r1->focus->y;
	D[2][0] = 1;
	X[0][1] = (pow(r2->focus->x, 2) + pow(r2->focus->y, 2));
	X[1][1] = r2->focus->y;
	X[2][1] = 1;
	Y[0][1] = r2->focus->x;
	Y[1][1] = (pow(r2->focus->x, 2) + pow(r2->focus->y, 2));
	Y[2][1] = 1;
	D[0][1] = r2->focus->x;
	D[1][1] = r2->focus->y;
	D[2][1] = 1;
	X[0][2] = (pow(r3->focus->x, 2) + pow(r3->focus->y, 2));
	X[1][2] = r3->focus->y;
	X[2][2] = 1;
	Y[0][2] = r3->focus->x;
	Y[1][2] = (pow(r3->focus->x, 2) + pow(r3->focus->y, 2));
	Y[2][2] = 1;
	D[0][2] = r3->focus->x;
	D[1][2] = r3->focus->y;
	D[2][2] = 1;
	double DX = calculator::det(X);
	double DY = calculator::det(Y);
	double DD = calculator::det(D);
	center = new PointF((DX / (2 * DD)), (DY / (2 * DD)));
	// find max radius to avoid errors
	double r = std::max(calculator::distance(r1->focus, center), calculator::distance(r2->focus, center));
	r = std::max(r, calculator::distance(r3->focus, center));
	this->x = center.x + r;
	isCircle = true;
}

Event::~Event()
{

}

SweepLine::SweepLine()
{
}

SweepLine::SweepLine(Voronoi* vmap)
{
	this->vmap = vmap;
	for(int i = 0; i < this->vmap->polygons.size(); ++i){
		Polygon* bar = this->vmap->polygons[i];
		bar->id = i;
		addSite(new Event(bar));
	}
}

SweepLine::~SweepLine()
{

}

void SweepLine::addSite(Event *eve)
{
	siteEvents.push(eve);
}

double SweepLine::nextEvent()
{
	L = LMAXVALUE;
	// no event left
	if(siteEvents.size() + circleEvents.size() == 0){
		return L;
	}
	// get next event location
	// might be site event or circle event
	Event* nextEvent = nullptr;
	if(siteEvents.size() > 0){
		nextEvent = siteEvents.top();
		if(circleEvents.size() > 0 && nextEvent->x >= circleEvents.top()->x){
			nextEvent = circleEvents.top();
		}
	}
	L = nextEvent->x;
	if(!nextEvent->isCircle){
		// site event
		beachAdd(nextEvent->relevants[0]);
		siteEvents.pop();
	}else{
		// circle event
		auto point = nextEvent->center;
		int pi = nextEvent->relevants[0]->poly->id;
		int pj = nextEvent->relevants[1]->poly->id;
		int pk = nextEvent->relevants[2]->poly->id;
		// usually pj is the one that'll be deleted from beachLine
		for(voronoiMap::Edge* edge : nextEvent->relevants[0]->poly->edges){
			if(edge->hasParentID(pj)){
				if(edge->isAbstract()){
					edge->deAbstract();
					edge->a = new voronoiMap::Point(point);
				}else{
					edge->b = new voronoiMap::Point(point);
				}
			}
		}
		for(voronoiMap::Edge* edge : nextEvent->relevants[2]->poly->edges){
			if(edge->hasParentID(pj)){
				if(edge->isAbstract()){
					edge->deAbstract();
					edge->a = new voronoiMap::Point(point);
				}else{
					edge->b = new voronoiMap::Point(point);
				}
			}
		}
		for(voronoiMap::Edge* edge : nextEvent->relevants[1]->poly->edges){
			if(edge->hasParentID(pi) || edge->hasParentID(pk)){
				if(edge->isAbstract()){
					edge->deAbstract();
					edge->a = new voronoiMap::Point(point);
				}else{
					edge->b = new voronoiMap::Point(point);
				}
			}
		}
		Edge* bar = new Edge(pi, pk);
		bar->a = new Point(point);
		nextEvent->relevants[0]->poly->edges.push_back(new Edge(bar));
		nextEvent->relevants[2]->poly->edges.push_back(bar);
		dealCircleEvent(nextEvent);
	}
	return L;
}

void SweepLine::beachAdd(Parabola *para)
{
	int pos = 0;
	Parabola *pj = nullptr;
	for (int i = 0; i < beachParas.size() - 2; ++i) {
		auto intersect = getIntersect(beachParas[i]->focus, beachParas[i + 1]->focus);
		auto intersect2 = getIntersect(beachParas[i + 1]->focus, beachParas[i + 2]->focus);
		if(intersect.y < para->focus->y && intersect2.y < para->focus->y){
			pos = i + 1;
			break;
		}
		else if(i == 0 && intersect.y > para->focus->y){
			// first one
			pos = i;
			break;
		}else if(i == beachParas.size() - 2 - 1){
			// last one
			pos = beachParas.size() - 1;
			break;
		}
	}
	if(beachParas.size() == 0){
		// beach is empty
		beachParas.push_back(para);
		return;
	}
	pj = beachParas[pos];
	if(pj->focus->x == para->focus->x){
		// special case, first few point on same x pos
		if(pj->focus->y > para->focus->y)
			beachParas.insert(beachParas.begin() + pos, para);
		else
			beachParas.insert(beachParas.begin() + pos + 1, para);
		auto bar = new voronoiMap::Edge(para->poly->id, pj->poly->id);
		bar->a = new voronoiMap::Point(-vmap->width, (pj->focus->y + para->focus->y) / 2);
		para->poly->edges.push_back(new voronoiMap::Edge(bar));
		pj->poly->edges.push_back(bar);
		return;
	}else if(beachParas.size() == 2){
		// special case, on third point insertion if first two point is on same x pos
		if(para->focus->y < beachParas[0]->focus->y)
			pos = 0;
		else if(para->focus->y > beachParas[0]->focus->y && para->focus->y < beachParas[1]->focus->y)
			if(para->focus->y >= getIntersect(beachParas[0]->focus, beachParas[1]->focus).y){
				pos = 1;
			}else{
				pos = 0;
			}
		else if(para->focus->y > beachParas[1]->focus->y)
			pos = 1;
		pj = beachParas[pos];
	}
	Parabola* pj_l = nullptr;
	Parabola* pj_r = nullptr;
	if(pos - 1 >= 0)
		pj_l = beachParas[pos - 1];
	if(pos + 1 < beachParas.size())
		pj_r = beachParas[pos + 1];
	beachParas.insert(beachParas.begin() + pos + 1, para);
	beachParas.insert(beachParas.begin() + pos + 2, pj);
	// now pi is on pos + 1
	pos++;

	// add to edges
	para->poly->edges.push_back(new voronoiMap::Edge(para->poly->id, pj->poly->id, true));
	pj->poly->edges.push_back(new voronoiMap::Edge(para->poly->id, pj->poly->id, true));

	circleEvents.removeAll([&](Event* e){
		if(e->relevants[0] == pj_l && e->relevants[1] == pj && e->relevants[2] == pj_r)
			return true;
		return false;
	});
	// adding new triple involving para (which is left 2 & right 2 of para with para)
	for (int j = pos - 1; j <= pos + 1; j += 2) {
		if((j - 1) < 0 || (j + 1) >= beachParas.size())
			continue;
		Parabola* p1 = beachParas[j - 1];
		Parabola* p2 = beachParas[j];
		Parabola* p3 = beachParas[j + 1];
		// b to a cross c to a is smaller than zero means it's a left turn
		if((p2->focus->x - p1->poly->focus->x) * (p3->poly->focus->y - p1->poly->focus->y) - (p3->poly->focus->x - p1->poly->focus->x) * (p2->poly->focus->y - p1->poly->focus->y) < 0){
			Event *circleEve = new Event(p1, p2, p3);
			if(circleEve->x >= L){
				circleEvents.push(circleEve);
			}
		}
	}
}

// delete e from circleEvents and corresponding p from beachLine
void SweepLine::dealCircleEvent(Event *eve)
{
	bool err = true;
	int p1 = eve->relevants[0]->poly->id;
	int p2 = eve->relevants[1]->poly->id;
	int p3 = eve->relevants[2]->poly->id;
	for (int i = 0; i < beachParas.size() - 2; ++i) {
		if(beachParas[i]->poly->id == p1 && beachParas[i + 1]->poly->id == p2 && beachParas[i + 2]->poly->id == p3){
			if(i > 0){
				Parabola* pp1 = beachParas[i - 1];
				Parabola* pp2 = beachParas[i];
				Parabola* pp3 = beachParas[i + 1];
				if((pp2->focus->x - pp1->focus->x) * (pp3->focus->y - pp1->focus->y) - (pp3->focus->x - pp1->focus->x) * (pp2->focus->y - pp1->focus->y) < 0){
					Event* cirEve = new Event(pp1, pp2, pp3);
					if(cirEve->x >= L){
						circleEvents.push(cirEve);
					}
				}
			}
			if(i < beachParas.size() - 3){
				Parabola* pp1 = beachParas[i];
				Parabola* pp2 = beachParas[i + 2];
				Parabola* pp3 = beachParas[i + 3];
				if((pp2->focus->x - pp1->focus->x) * (pp3->focus->y - pp1->focus->y) - (pp3->focus->x - pp1->focus->x) * (pp2->focus->y - pp1->focus->y) < 0){
					Event* cirEve = new Event(pp1, pp2, pp3);
					if(cirEve->x >= L){
						circleEvents.push(cirEve);
					}
				}
			}
			err = false;
			beachParas.erase(beachParas.begin() + i + 1);
			break;
		}
	}
	circleEvents.remove(eve);
	circleEvents.removeAll([&](Event* e2){
		if(e2->relevants[0] == eve->relevants[1] && e2->relevants[1] == eve->relevants[2])
			return true;
		if(e2->relevants[1] == eve->relevants[0] && e2->relevants[2] == eve->relevants[1])
			return true;
		return false;
	});

	if(err)
		fprintf(stderr, "Error dealCircleEvent! no matching pattern in beach");
	return;
}

void SweepLine::finishEdges()
{
	// set L large enough
	L = 2 * vmap->width + 2 * vmap->height;
	for (int i = 0; i < beachParas.size() - 1; i++)
	{
		Polygon* p1 = beachParas[i]->poly;
		Polygon* p2 = beachParas[i + 1]->poly;
		PointF cross = getIntersect((PointF*)p1->focus, (PointF*)p2->focus);
		for(auto &edge : p1->edges)
		{
			if(edge->hasParentID(p2->id))
			{
				edge->b = new Point(cross);
			}
		}
		for(auto &edge : p2->edges)
		{
			if(edge->hasParentID(p1->id))
			{
				edge->b = new Point(cross);
			}
		}
	}
}

double SweepLine::parabolaX(const Point& focus, double y)
{
	double k = focus.y;
	double h = (L + focus.x) / 2.0;
	double c = -(L - focus.x) / 2.0;
	if (c >= 0)
	{
		return LMINVALUE;
	}
	else
	{
		double x = pow(y - k, 2) / (4 * c) + h;
		return x;
	}
}

PointF SweepLine::getIntersect(PointF *A, PointF *B)
{
	double ka = A->y;
	double ha = (L + A->x) / 2.0;
	double ca = -(L - A->x) / 2.0;
	double kb = B->y;
	double hb = (L + B->x) / 2.0;
	double cb = -(L - B->x) / 2.0;
	double a = cb - ca;
	double b = -2 * (cb * ka - ca * kb);
	double c = -(4 * ca * cb * (hb - ha) - cb * ka * ka + ca * kb * kb);
	double distance = b * b - 4 * a * c;
	if (a == 0)
	{
		if (b == 0)
		{
			return PointF(LMINVALUE, (A->y + B->y) / 2);
		}
		double y = -c / b;
		double x = pow(y - ka, 2) / (4 * ca) + ha;
		return new PointF((float)x, (float)y);
	}
	else
	{
		double root[2];
		root[0] = (-b + sqrt(distance)) / (2 * a);
		root[1] = (-b - sqrt(distance)) / (2 * a);
		std::sort(root, root + 2);
		double y;
		if (A->x < B->x)
		{
			y = root[0];
		}
		else
		{
			y = root[1];
		}
		double x = pow(y - ka, 2) / (4 * ca) + ha;
		return PointF(x, y);
	}
}

double calculator::det(double m[3][3]){
	return (m[0][0] * m[1][1] * m[2][2] + m[0][1] * m[1][2] * m[2][0] + m[0][2] * m[1][0] * m[2][1])
			- (m[0][2] * m[1][1] * m[2][0] + m[0][1] * m[1][0] * m[2][2] + m[0][0] * m[1][2] * m[2][1]);
}

double calculator::distance(PointF a, PointF b)
{
	return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}
