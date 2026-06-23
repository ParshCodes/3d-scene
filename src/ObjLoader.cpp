#include "ObjLoader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>
#include <glm/glm.hpp>

struct FaceVert {
    int p = 0, t = 0, n = 0;
    bool operator==(const FaceVert& o) const { return p == o.p && t == o.t && n == o.n; }
};

struct FaceVertHash {
    size_t operator()(const FaceVert& fv) const {
        return std::hash<int>()(fv.p) ^ (std::hash<int>()(fv.t) << 12) ^ (std::hash<int>()(fv.n) << 24);
    }
};

static FaceVert parseFaceVert(const std::string& token) {
    FaceVert fv;
    std::istringstream ss(token);
    std::string part;
    int idx = 0;
    while (std::getline(ss, part, '/')) {
        if (!part.empty()) {
            int val = std::stoi(part);
            if (idx == 0) fv.p = val;
            else if (idx == 1) fv.t = val;
            else if (idx == 2) fv.n = val;
        }
        ++idx;
    }
    return fv;
}

std::unique_ptr<Mesh> loadObj(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "[OBJ] Cannot open: " << path << "\n";
        return nullptr;
    }

    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> uvs;

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::unordered_map<FaceVert, unsigned int, FaceVertHash> cache;

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;

        std::istringstream ss(line);
        std::string token;
        ss >> token;

        if (token == "v") {
            glm::vec3 p; ss >> p.x >> p.y >> p.z;
            positions.push_back(p);
        } else if (token == "vn") {
            glm::vec3 n; ss >> n.x >> n.y >> n.z;
            normals.push_back(n);
        } else if (token == "vt") {
            glm::vec2 uv; ss >> uv.x >> uv.y;
            uvs.push_back(uv);
        } else if (token == "f") {
            std::vector<FaceVert> corners;
            std::string word;
            while (ss >> word)
                corners.push_back(parseFaceVert(word));

            // fan triangulation: (0,1,2), (0,2,3), ...
            for (size_t i = 1; i + 1 < corners.size(); ++i) {
                for (const FaceVert& fv : { corners[0], corners[i], corners[i + 1] }) {
                    auto it = cache.find(fv);
                    if (it != cache.end()) {
                        indices.push_back(it->second);
                    } else {
                        Vertex v{};
                        if (fv.p > 0 && fv.p <= (int)positions.size())
                            v.position = positions[fv.p - 1];
                        if (fv.n > 0 && fv.n <= (int)normals.size())
                            v.normal = normals[fv.n - 1];
                        if (fv.t > 0 && fv.t <= (int)uvs.size())
                            v.texCoord = uvs[fv.t - 1];

                        unsigned int idx = (unsigned int)vertices.size();
                        vertices.push_back(v);
                        cache[fv] = idx;
                        indices.push_back(idx);
                    }
                }
            }
        }
    }

    if (normals.empty()) {
        for (size_t i = 0; i + 2 < indices.size(); i += 3) {
            glm::vec3 e1 = vertices[indices[i + 1]].position - vertices[indices[i]].position;
            glm::vec3 e2 = vertices[indices[i + 2]].position - vertices[indices[i]].position;
            glm::vec3 n  = glm::normalize(glm::cross(e1, e2));
            vertices[indices[i    ]].normal = n;
            vertices[indices[i + 1]].normal = n;
            vertices[indices[i + 2]].normal = n;
        }
    }

    std::cout << "[OBJ] " << path
              << " — " << vertices.size() << " verts, "
              << indices.size() / 3 << " tris\n";

    return std::make_unique<Mesh>(vertices, indices);
}
