#ifndef _TRACK_H_
#define _TRACK_H_
#include "BezierCurve.h"
#include "AnchorPoint.h"
#include "TangentPoint.h"
#include "TangentLine.h"

class Track : public Geometry {
	std::vector<BezierCurve *> tracks;
	std::vector<AnchorPoint *> reds;
	std::vector<TangentPoint *> greens;
	std::vector<TangentLine *> tangents;
	unsigned highlightID = 0;
	Geometry *highlightNode = nullptr;
	void highlight(Geometry *node);
	void dehighlight(Geometry *node, unsigned id);
public:
	Track();
	~Track();
	void draw(GLuint shader_color);
	void highlight_next();
	void highlight_prev();
	void highlight_move(glm::vec3 v);
	void acceleration(unsigned id);
	void deacceleration(unsigned id);
	void sphere_update(Geometry *sphere, unsigned seg_id, unsigned pt_id);
	BezierCurve *get_track(unsigned ind) { return tracks[ind % 8]; }
	unsigned size() { return tracks.size(); }
};

#endif