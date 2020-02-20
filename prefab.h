// Set prefab's properties, generate objects and give them events. Once done call prefab::generate()

#include<string>
#include<vector>
#include<fstream>
using namespace std;

struct Object;

namespace prefab {
	string name;
	int type = 0;
	float offset = 0.;
	vector<Object*> objects;
	ofstream file;
	void indent(int n) {
		for (int i = 0; i < n; i++)
			file << "   ";
	}
	template <typename T>
	void indent(int n, string s, T param) {
		for (int i = 0; i < n; i++)
			file << "   ";
		file << "\"" + s + "\" : \"" << param << "\", \n";
	}
	void generate();
}

namespace prefabTypes {
	enum {
		Bomb = 0,
		Bullet = 1,
		Beam = 2,
		Spinner = 3,
		Pulse = 4,
		Character = 5,
		Misc1 = 6,
		Misc2 = 7,
		Misc3 = 8,
		Misc4 = 9
	};
}

namespace eventTypes {
	const string pos = "pos";
	const string sca = "sca";
	const string rot = "rot";
	const string col = "col";
}

namespace easeTypes {
	const string line = "Linear";
	const string inst = "Instant";
	const string iSine = "InSine";
	const string oSine = "OutSine";
	const string ioSine = "InOutSine";
	const string iElas = "InElastic";
	const string oElas = "OutElastic";
	const string ioElas = "InOutElastic";
	const string iBack = "InBack";
	const string oBack = "OutBack";
	const string ioBack = "InOutBack";
	const string iBoun = "InBounce";
	const string oBoun = "OutBounce";
	const string ioBoun = "InOutBounce";
}
namespace et = easeTypes;


namespace shapes {
	struct Shape {
		int shape, so;
		string text;
		Shape(int shape, int so = 0, string text = "") {
			this->shape = shape;
			this->so = so;
			this->text = text;
		}
	};
	const Shape* const sq = new Shape(0); //Square
	const Shape* const sq2 = new Shape(0, 1); //Hollow Square
	const Shape* const sq3 = new Shape(0, 2); //Hollow Square
	const Shape* const cr = new Shape(1);//Circle
	const Shape* const cr2 = new Shape(1, 1);
	const Shape* const cr3 = new Shape(1, 4);
	const Shape* const sCr = new Shape(1, 2);//Semicircle
	const Shape* const sCr2 = new Shape(1, 3);
	const Shape* const qCr = new Shape(1, 5);//Quad Circle
	const Shape* const qCr2 = new Shape(1, 6);
	const Shape* const oCr = new Shape(1, 7);//Oct Circle
	const Shape* const oCr2 = new Shape(1, 8);
	const Shape* const tr = new Shape(2);
	const Shape* const tr2 = new Shape(2, 1);
	const Shape* const rTr = new Shape(2, 2);//Right Triangle
	const Shape* const rTr2 = new Shape(2, 3);
	const Shape* const circ = new Shape(1);
	const Shape* const ar = new Shape(3);//Arrow
	const Shape* const ar2 = new Shape(3, 1);
	const Shape* const hx = new Shape(5);
	const Shape* const hx2 = new Shape(5, 1);
	const Shape* hx3 = new Shape(5, 2);
	const Shape* sHx = new Shape(5, 3);//Semi Hexagon
	const Shape* sHx2 = new Shape(5, 4);
	Shape* txt(string s) {
		return new Shape(4, 0, s);
	}
}

struct Event {
	string eventType;
	float t, x, y;
	int r;
	float rx, ry, rz;
	string easeType;
	Event(string type, float t, float x, float y, int r, float rx, float ry, float rz, string easeType) {
		eventType = type;
		this->t = t;
		this->x = x;
		this->y = y;
		this->r = r;
		this->rx = rx;
		this->ry = ry;
		this->rz = rz;
		this->easeType = easeType;
	}
	void print() {
		using namespace prefab;
		indent(5);
		file << "{ \n";
		indent(6, "t", t);
		indent(6, "x", x);
		if (eventType == eventTypes::pos || eventType == eventTypes::sca)
			indent(6, "y", y);
		if (r) {
			indent(6, "r", r);
			indent(6, "rx", rx);
			if (eventType == eventTypes::pos || eventType == eventTypes::sca)
				indent(6, "ry", ry);
			if (r != 2)
				indent(6, "rz", rz);
		}
		if (easeType != "" && easeType != easeTypes::line)
			indent(6, "ct", easeType);
		indent(5);
		file << "}";
	}
};

struct Object {
	string id, name, pt;
	Object* parent = NULL;
	int depth = 15;
	shapes::Shape* shape = NULL;
	bool helper = false;
	bool autoKill = false;
	bool empty = false;
	float st = 0.;
	float ox = 0.;
	float oy = 0.;
	int bin = 0;
	int layer = 1;
	float posOffset = 0.;
	float scaOffset = 0.;
	float rotOffset = 0.;
	vector<Event> pos;
	vector<Event> sca;
	vector<Event> rot;
	vector<Event> col;
	Object(float st, int depth, const shapes::Shape* shape, int bin = 0, bool helper = false, bool autoKill = true, bool empty = false) {
		this->st = st;
		this->depth = depth;
		this->shape = (shapes::Shape*) shape;
		this->bin = bin;
		this->helper = helper;
		this->autoKill = autoKill;
		this->empty = empty;
		this->pt = "101";
		id = std::to_string(prefab::objects.size());
		prefab::objects.push_back(this);
	}
	void setCenter(float ox, float oy) {
		this->ox = ox;
		this->oy = oy;
	}
	void parentOffset(bool posBool, bool scaBool, bool rotBool, float posOffset = 0., float scaOffset = 0., float rotOffset = 0.) {
		pt = posBool ? "1" : "0";
		pt += scaBool ? "1" : "0";
		pt += rotBool ? "1" : "0";
		this->posOffset = posOffset;
		this->scaOffset = scaOffset;
		this->rotOffset = rotOffset;
	}
	void posEvent(float t, float x, float y, string easeType = easeTypes::line, int r = 0, float rx = 0., float ry = 0., float rz = 0.) {
		pos.push_back(Event(eventTypes::pos, t, x, y, r, rx, ry, rz, easeType));
	}
	void scaEvent(float t, float x, float y, string easeType = easeTypes::line, int r = 0, float rx = 0., float ry = 0., float rz = 0.) {
		sca.push_back(Event(eventTypes::sca, t, x, y, r, rx, ry, rz, easeType));
	}
	void rotEvent(float t, float x, string easeType = easeTypes::line, int r = 0, float rx = 0., float rz = 0.) {
		rot.push_back(Event(eventTypes::rot, t, x, 0., r, rx, 0., rz, easeType));
	}
	void colEvent(float t, int x, string easeType = easeTypes::line, int r = 0, float rx = 0., float rz = 0.) {
		col.push_back(Event(eventTypes::col, t, x, 0., r, rx, 0., rz, easeType));
	}
	void defaultStart() {//for empty objects
		posEvent(0., 0., 0.);
		scaEvent(0., 1., 1.);
		rotEvent(0., 0.);
		colEvent(0., 0);
	}
	void addEvent(Event event) {
		if (event.eventType == eventTypes::pos)
			pos.push_back(event);
		else if (event.eventType == eventTypes::sca)
			sca.push_back(event);
		else if (event.eventType == eventTypes::rot)
			rot.push_back(event);
		else if (event.eventType == eventTypes::col)
			col.push_back(event);
	}
	void print() {
		using namespace prefab;
		indent(2);
		file << "{ \n";
		indent(3, "id", id);
		if (pt != "101")
			indent(3, "pt", pt);
		if (posOffset || scaOffset || rotOffset) {
			indent(3);
			file << "\"po\" : [\n";
			indent(4);
			file << '\"' << posOffset << "\",\n";
			indent(4);
			file << '\"' << scaOffset << "\",\n";
			indent(4);
			file << '\"' << rotOffset << "\"\n";
			indent(4);
			file << "],\n";
		}
		indent(3, "p", parent ? parent->id : "");
		indent(3, "d", depth);
		indent(3, "h", helper ? "True" : "False");
		indent(3, "st", st);
		indent(3, "name", name);
		if (shape && (shape->shape || shape->so)) {
			indent(3, "shape", shape->shape);
			indent(3, "so", shape->so);
		}
		if (autoKill)
			indent(3, "ak", "True");
		if (empty)
			indent(3, "empty", "True");
		indent(3);
		file << "\"o\" : { \n";
		indent(4, "x", ox);
		indent(4, "y", oy);
		indent(3);
		file << "}, \n";
		indent(3);
		file << "\"ed\" : { \n";
		indent(4, "bin", bin);
		indent(4, "layer", layer);
		indent(3);
		file << "}, \n";
		indent(3);
		file << "\"events\" : {\n";
		indent(4);
		file << "\"pos\" : [\n";
		bool first = true;
		for (Event& event : pos) {
			if (first)
				first = false;
			else
				file << ",\n";
			event.print();
		}
		file << endl;
		indent(4);
		file << "], \n";
		indent(4);
		file << "\"sca\" : [\n";
		first = true;
		for (Event& event : sca) {
			if (first)
				first = false;
			else
				file << ",\n";
			event.print();
		}
		file << endl;
		indent(4);
		file << "], \n";
		indent(4);
		file << "\"rot\" : [\n";
		first = true;
		for (Event& event : rot) {
			if (first)
				first = false;
			else
				file << ",\n";
			event.print();
		}
		file << endl;
		indent(4);
		file << "], \n";
		indent(4);
		file << "\"col\" : [\n";
		first = true;
		for (Event& event : col) {
			if (first)
				first = false;
			else
				file << ",\n";
			event.print();
		}
		file << endl;
		indent(4);
		file << "] \n";
		indent(3);
		file << "} \n";
		indent(2);
		file << "}";
	}
};


void prefab::generate() {
	string fileName = "";
	for (char c : name)
		fileName += tolower(c);
	fileName += ".lsp";
	file.open(fileName);
	file << "{\n";
	indent(1, "name", name);
	indent(1, "type", type);
	indent(1, "offset", offset);
	indent(1);
	file << "\"objects\" : [ \n";
	bool first = true;
	for (Object* object : objects) {
		if (first)
			first = false;
		else
			file << ",\n";
		object->print();
	}
	file << endl;
	indent(1);
	file << "] \n}";
	file.close();
}