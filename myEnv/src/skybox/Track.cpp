#include "Track.h"
#define MAX_ID 25

/* Track resources */
std::vector<std::vector<glm::vec3>> trackPoints = {
	trackPoints1, trackPoints2, trackPoints3, trackPoints4,
	trackPoints5, trackPoints6, trackPoints7, trackPoints8
};

Track::Track() {

	glm::vec3 lastControlPoint = trackPoints[trackPoints.size() - 1][2];

	for (std::vector<glm::vec3> trackP : trackPoints) {

		// create curves
		tracks.push_back(new BezierCurve(trackP));
		tracks[tracks.size() - 1]->enable_draw_line();
		tracks[tracks.size() - 1]->mycolor_set(glm::vec3(0));

		// scale curves
		//for (auto &p : tracks)
		//	p->scale(10.0f);

		// create anchor points
		reds.push_back(new AnchorPoint(ObjPath_Sphere,
			trackP[0], 1.0f, true, glm::vec3(1, 0, 0)));

		// create tangent points
		greens.push_back(new TangentPoint(ObjPath_Sphere,
			trackP[1], 1.0f, true, glm::vec3(0, 1, 0)));
		greens.push_back(new TangentPoint(ObjPath_Sphere,
			trackP[2], 1.0f, true, glm::vec3(0, 1, 0)));

		// create tangents
		tangents.push_back(new TangentLine(lastControlPoint,
			trackP[1]));
		lastControlPoint = trackP[2];
	}

#ifndef DEBUG2
	/* Create linkedlist */
	for (unsigned i = 0; i != reds.size(); ++i) {
		// Link with front node
		if (i > 0) {
			reds[i]->set_prev(greens[2 * i - 1]);
			greens[2 * i - 1]->set_next(reds[i]);
		} else {
			reds[i]->set_prev(greens[greens.size() - 1]);
			greens[greens.size() - 1]->set_next(reds[i]);
		}

		// Link green nodes with red nodes
		reds[i]->set_next(greens[2*i]);
		greens[2 * i]->set_prev(reds[i]);

		greens[2 * i]->set_next(greens[2 * i + 1]);
		greens[2 * i + 1]->set_prev(greens[2 * i]);
	}
#endif

	/* Highlight */
	highlightNode = reds[0];
	highlight(highlightNode);
}

Track::~Track() {
	for (unsigned i = 0; i != tracks.size(); ++i)
		delete tracks[i];
	for (unsigned i = 0; i != reds.size(); ++i)
		delete reds[i];
	for (unsigned i = 0; i != greens.size(); ++i)
		delete greens[i];
	for (unsigned i = 0; i != tangents.size(); ++i)
		delete tangents[i];
}

void Track::draw(GLuint shader_color) {
	for (auto seg : tracks)
		seg->draw(shader_color, glm::mat4(1));
	for (auto r : reds)
		r->draw(shader_color, glm::mat4(1));
	for (auto g : greens)
		g->draw(shader_color, glm::mat4(1));
	for (auto tan : tangents)
		tan->draw(shader_color, glm::mat4(1));
}

void Track::highlight(Geometry *node) {
	node->mycolor_set(glm::vec3(1, 1, 1));
}

void Track::dehighlight(Geometry *node, unsigned id) {
	if (id % 3)
		node->mycolor_set(glm::vec3(0, 1, 0));
	else
		node->mycolor_set(glm::vec3(1, 0, 0));
}

void Track::highlight_next() {
	// de-emphasize the previous node
	dehighlight(highlightNode, highlightID);

	// highlight the next node
	highlightNode = highlightNode->get_next();
	highlightID = (highlightID+1) % 24;
	highlight(highlightNode);

	if (highlightNode == nullptr)
		std::cout << "error!" << std::endl;
}

void Track::highlight_prev() {
	// de-emphasize the previous node
	dehighlight(highlightNode, highlightID);

	// highlight the next node
	highlightNode = highlightNode->get_prev();
	if (highlightID == 0)
		highlightID = 24;
	--highlightID;
	highlight(highlightNode);
}

void Track::highlight_move(glm::vec3 v) {

	unsigned tangent_id = 0;
	unsigned curve_id1 = 0, curve_id2 = 0;
	glm::vec3 tan_first, tan_second;

	highlightNode->point_move(v);

	switch (highlightID % 3) {
		// red node
	case 0:
		// Control point
		highlightNode->get_next()->point_move(v);
		highlightNode->get_prev()->point_move(v);
		// Tangent Line
		tangent_id = highlightID / 3;
		tan_first = tangents[tangent_id]->get_first() + v;
		tan_second = tangents[tangent_id]->get_second() + v;
		// Bezier Curve
		curve_id1 = (tangent_id + 7) % 8;
		curve_id2 = tangent_id;
		tracks[curve_id1]->movePoint(v, 2);
		tracks[curve_id1]->movePoint(v, 3);
		tracks[curve_id1]->updateParameter();
		tracks[curve_id2]->movePoint(v, 0);
		tracks[curve_id2]->movePoint(v, 1);
		tracks[curve_id2]->updateParameter();
		break;
		// Green node #1
	case 1:
		highlightNode->get_prev()->get_prev()->point_move(-1.0f * v);
		tangent_id = highlightID / 3;
		tan_first = tangents[tangent_id]->get_first() - v;
		tan_second = tangents[tangent_id]->get_second() + v;
		// Bezier Curve
		curve_id1 = (tangent_id + 7) % 8;
		curve_id2 = tangent_id;
		tracks[curve_id1]->movePoint(-1.0f*v, 2);
		tracks[curve_id1]->updateParameter();
		tracks[curve_id2]->movePoint(v, 1);
		tracks[curve_id2]->updateParameter();
		break;
		// Green node #2
	case 2:
		highlightNode->get_next()->get_next()->point_move(-1.0f * v);
		tangent_id = (highlightID / 3 + 1) % 8;
		tan_first = tangents[tangent_id]->get_first() + v;
		tan_second = tangents[tangent_id]->get_second() - v;
		// Bezier Curve
		curve_id1 = tangent_id;
		curve_id2 = (tangent_id + 1) % 8;
		tracks[curve_id1]->movePoint(v, 2);
		tracks[curve_id1]->updateParameter();
		tracks[curve_id2]->movePoint(-1.0f*v, 1);
		tracks[curve_id2]->updateParameter();
		break;
	}

	tangents[tangent_id]->update(tan_first, tan_second);	
}

void Track::sphere_update(Geometry *sphere, unsigned seg_id, unsigned pt_id) {
	glm::vec3 new_position = tracks[seg_id]->get_point(pt_id);
	glm::vec3 old_position = sphere->get_position();

	// std::cout << new_position.x << " " << new_position.y << " " << new_position.z << std::endl;
	// std::cout << old_position.x << " " << old_position.y << " " << old_position.z << std::endl;

	sphere->move(new_position - old_position);
}

void Track::acceleration(unsigned seg_id) {
	// highlight by organe
	tracks[seg_id]->mycolor_set(glm::vec3(1, 0.5f, 0));
}

void Track::deacceleration(unsigned seg_id) {
	// dehighlight to black
	tracks[seg_id]->mycolor_set(glm::vec3(0, 0, 0));
}
