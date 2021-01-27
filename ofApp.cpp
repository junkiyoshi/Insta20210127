#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openFrameworks");

	ofBackground(239);
	ofSetLineWidth(1.5);
	ofEnableDepthTest();

	this->line.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);
}

//--------------------------------------------------------------
void ofApp::update() {

	// Parameter reference by https://twitter.com/ru_sack/status/798308368007892992

	this->face.clear();
	this->line.clear();

	float a = 1.25;
	float b = 1.25;
	float c = 1.0;
	float h = 3.5;
	float k = 0;
	float w = 0.12;
	int R = 1;
	int scale = 40;

	float u_step = 0.3;
	int v_step = 15;

	int rotate_deg = ofGetFrameNum();

	for (int x = -300; x <= 300; x += 300) {

		for (int y = -300; y <= 300; y += 300) {

			rotate_deg += 40;
			float angle = rotate_deg * (float)DEG_TO_RAD;
			auto rotation = glm::rotate(glm::mat4(), angle, glm::vec3(0, 1, 0));

			for (float u = -35; u <= -1; u += u_step) {

				for (int v = 0; v <= 360; v += v_step) {

					auto noise_location = glm::vec2(x + cos(v * DEG_TO_RAD), y + sin(v * DEG_TO_RAD));
					if (ofNoise(glm::vec4(u * 0.005, noise_location * 0.5, ofGetFrameNum() * 0.01)) < 0.5) { continue; }

					vector<glm::vec3> vertices;
					vertices.push_back(this->make_shell_point(a, b, c, h, k, u + u_step * 0.5, (v + v_step * 0.5) * DEG_TO_RAD, w, R) * scale); // -4
					vertices.push_back(this->make_shell_point(a, b, c, h, k, u + u_step * 0.5, (v - v_step * 0.5) * DEG_TO_RAD, w, R) * scale); // -3
					vertices.push_back(this->make_shell_point(a, b, c, h, k, u - u_step * 0.5, (v - v_step * 0.5) * DEG_TO_RAD, w, R) * scale); // -2
					vertices.push_back(this->make_shell_point(a, b, c, h, k, u - u_step * 0.5, (v + v_step * 0.5) * DEG_TO_RAD, w, R) * scale); // -1

					for (auto& vertex : vertices) {

						vertex = glm::vec3(x, y, 0) + glm::vec4(vertex, 0) * rotation;
					}

					this->face.addVertices(vertices);

					this->face.addIndex(this->face.getNumVertices() - 1); this->face.addIndex(this->face.getNumVertices() - 2); this->face.addIndex(this->face.getNumVertices() - 3);
					this->face.addIndex(this->face.getNumVertices() - 1); this->face.addIndex(this->face.getNumVertices() - 3); this->face.addIndex(this->face.getNumVertices() - 4);

					this->line.addVertices(vertices);

					this->line.addIndex(this->line.getNumVertices() - 1); this->line.addIndex(this->line.getNumVertices() - 2);
					this->line.addIndex(this->line.getNumVertices() - 1); this->line.addIndex(this->line.getNumVertices() - 4);
					this->line.addIndex(this->line.getNumVertices() - 3); this->line.addIndex(this->line.getNumVertices() - 2);
					this->line.addIndex(this->line.getNumVertices() - 3); this->line.addIndex(this->line.getNumVertices() - 4);
				}
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();

	ofSetColor(239);
	this->face.draw();

	ofSetColor(39);
	this->line.draw();

	this->cam.end();
}

//--------------------------------------------------------------
glm::vec3 ofApp::make_shell_point(float a, float b, float c, float h, float k, float u, float v, float w, int R) {

	// 数学デッサン教室 描いて楽しむ数学たち 貝殻の形 shape of shell P.59

	float x = (h + a * cos(v)) * exp(w * u) * cos(c * u);
	float y = R * (h + a * cos(v)) * exp(w * u) * sin(c * u);
	float z = (k + b * sin(v)) * exp(w * u);

	return glm::vec3(x, y, z);
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}