#include <iostream>
#include <cmath>
#include <vector>

using namespace std;

// Define a simple 3D point structure
struct Point3D {
    float x, y, z;
};

// Define a simple 3D vector structure
struct Vector3D {
    float x, y, z;
    
    Vector3D operator-(const Vector3D& other) const {
        return {x - other.x, y - other.y, z - other.z};
    }
};

// Define a simple bounding box structure
struct BoundingBox {
    Point3D min, max;
};

// Define a simple 3D line segment structure
struct LineSegment3D {
    Point3D start, end;
};

// Function to check if a point is inside a bounding box
bool pointInBoundingBox(const Point3D& point, const BoundingBox& box) {
    return (point.x >= box.min.x && point.x <= box.max.x &&
            point.y >= box.min.y && point.y <= box.max.y &&
            point.z >= box.min.z && point.z <= box.max.z);
}

// Function to check if a ray intersects a 3D triangle
bool rayIntersectsTriangle(const Point3D& rayOrigin, const Vector3D& rayDir, const Point3D& v0, const Point3D& v1, const Point3D& v2) {
    Vector3D edge1 = v1 - v0;
    Vector3D edge2 = v2 - v0;
    Vector3D h = cross(rayDir, edge2);
    float a = dot(edge1, h);
    if (a > -0.00001 && a < 0.00001)
        return false; // Ray is parallel to triangle
    float f = 1.0f / a;
    Vector3D s = rayOrigin - v0;
    float u = f * dot(s, h);
    if (u < 0.0f || u > 1.0f)
        return false;
    Vector3D q = cross(s, edge1);
    float v = f * dot(rayDir, q);
    if (v < 0.0f || u + v > 1.0f)
        return false;
    // At this stage we can compute t to find out where the intersection point is on the line.
    float t = f * dot(edge2, q);
    if (t > 0.00001) // Ray intersection
        return true;
    else // This means that there is a line intersection but not a ray intersection.
        return false;
}

// Function to perform ray casting against a spinning cube
void castRay(const Point3D& rayOrigin, const Vector3D& rayDir, const vector<LineSegment3D>& cubeEdges) {
    const float cubeSize = 1.0f;
    const Point3D cubeCenter = {0.0f, 0.0f, 0.0f};
    const float halfCubeSize = cubeSize / 2.0f;
    
    Point3D vertices[8] = {
        {cubeCenter.x - halfCubeSize, cubeCenter.y - halfCubeSize, cubeCenter.z - halfCubeSize},
        {cubeCenter.x + halfCubeSize, cubeCenter.y - halfCubeSize, cubeCenter.z - halfCubeSize},
        {cubeCenter.x + halfCubeSize, cubeCenter.y + halfCubeSize, cubeCenter.z - halfCubeSize},
        {cubeCenter.x - halfCubeSize, cubeCenter.y + halfCubeSize, cubeCenter.z - halfCubeSize},
        {cubeCenter.x - halfCubeSize, cubeCenter.y - halfCubeSize, cubeCenter.z + halfCubeSize},
        {cubeCenter.x + halfCubeSize, cubeCenter.y - halfCubeSize, cubeCenter.z + halfCubeSize},
        {cubeCenter.x + halfCubeSize, cubeCenter.y + halfCubeSize, cubeCenter.z + halfCubeSize},
        {cubeCenter.x - halfCubeSize, cubeCenter.y + halfCubeSize, cubeCenter.z + halfCubeSize}
    };
    
    vector<Point3D> cubeFaces[6] = {
        {vertices[0], vertices[1], vertices[2], vertices[3]}, // Front face
        {vertices[4], vertices[5], vertices[6], vertices[7]}, // Back face
        {vertices[0], vertices[1], vertices[5], vertices[4]}, // Left face
        {vertices[3], vertices[2], vertices[6], vertices[7]}, // Right face
        {vertices[0], vertices[3], vertices[7], vertices[4]}, // Bottom face
        {vertices[1], vertices[2], vertices[6], vertices[5]}  // Top face
    };
    
    for (const auto& edge : cubeEdges) {
        for (const auto& face : cubeFaces) {
            if (rayIntersectsTriangle(rayOrigin, rayDir, face[0], face[1], face[2]) ||
                rayIntersectsTriangle(rayOrigin, rayDir, face[0], face[2], face[3])) {
                cout << "Ray intersects cube." << endl;
                return;
            }
        }
    }
    
    cout << "Ray does not intersect the cube." << endl;
}

// Function to compute cross product of two vectors
Vector3D cross(const Vector3D& a, const Vector3D& b) {
    return {a.y * b.z - a.z * b.y,
            a.z * b.x - a.x * b.z,
            a.x * b.y - a.y * b.x};
}

// Function to compute dot product of two vectors
float dot(const Vector3D& a, const Vector3D& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

int main() {
    // Define the ray origin and direction
    Point3D rayOrigin = {0.0f, 0.0f, -3.0f};
    Vector3D rayDirection = {0.0f, 0.0f, 1.0f}; // Example: ray pointing along the positive z-axis
    
    // Define the edges of a cube
    vector<LineSegment3D> cubeEdges = {
        {{-0.5f, -0.5f, -0.5f}, {0.5f, -0.5f, -0.5f}}, // Bottom edge
        {{0.5f, -0.5f, -0.5f}, {0.5f, 0.5f, -0.5f}},   // Right edge
        {{0.5f, 0.5f, -0.5f}, {-0.5f, 0.5f, -0.5f}},   // Top edge
        {{-0.5f, 0.5f, -0.5f}, {-0.5f, -0.5f, -0.5f}}, // Left edge
        {{-0.5f, -0.5f, 0.5f}, {0.5f, -0.5f, 0.5f}},   // Front edge
        {{0.5f, -0.5f, 0.5f}, {0.5f, 0.5f, 0.5f}},     // Right-front edge
        {{0.5f, 0.5f, 0.5f}, {-0.5f, 0.5f, 0.5f}},     // Top-front edge
        {{-0.5f, 0.5f, 0.5f}, {-0.5f, -0.5f, 0.5f}}    // Left-front edge
    };
    
    // Spin the cube around the y-axis
    const float rotationSpeed = 0.01f;
    float rotationAngle = 0.0f;

    // Main loop
    while (rotationAngle < 2 * M_PI) {
        // Update cube rotation
        for (auto& edge : cubeEdges) {
            float newXStart = edge.start.x * cos(rotationSpeed) + edge.start.z * sin(rotationSpeed);
            float newZStart = -edge.start.x * sin(rotationSpeed) + edge.start.z * cos(rotationSpeed);
            float newXEnd = edge.end.x * cos(rotationSpeed) + edge.end.z * sin(rotationSpeed);
            float newZEnd = -edge.end.x * sin(rotationSpeed) + edge.end.z * cos(rotationSpeed);
            edge.start.x = newXStart;
            edge.start.z = newZStart;
            edge.end.x = newXEnd;
            edge.end.z = newZEnd;
        }
        
        // Cast the ray and check for intersections
        castRay(rayOrigin, rayDirection, cubeEdges);
        
        // Update rotation angle
        rotationAngle += rotationSpeed;
    }
    
    return 0;
}
