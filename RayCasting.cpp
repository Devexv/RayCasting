#include <iostream>
#include <cmath>
#include <thread>
#include <chrono>

constexpr int consoleWidth = 80;
constexpr int consoleHeight = 24;

struct Vec3 {
    float x, y, z;

    Vec3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}

    Vec3 operator+(const Vec3& other) const {
        return Vec3(x + other.x, y + other.y, z + other.z);
    }

    Vec3 operator-(const Vec3& other) const {
        return Vec3(x - other.x, y - other.y, z - other.z);
    }

    Vec3 operator*(float scalar) const {
        return Vec3(x * scalar, y * scalar, z * scalar);
    }

    float dot(const Vec3& other) const {
        return x * other.x + y * other.y + z * other.z;
    }
};

class Cube {
private:
    Vec3 center;
    float size;

public:
    Cube(const Vec3& center_, float size_) : center(center_), size(size_) {}

    bool intersects(const Vec3& rayOrigin, const Vec3& rayDir, float& t) const {
        // The algorithm for ray-cube intersection is a bit more complex, 
        // but I'm simplifying it here for demonstration purposes.
        // You may want to implement a more robust algorithm.
        // Here's a basic approach for demonstration:

        // Compute t values for intersections with each face
        float tNear = -INFINITY;
        float tFar = INFINITY;

        for (int i = 0; i < 3; ++i) {
            float t1 = (center.x - size / 2 - rayOrigin.x) / rayDir.x;
            float t2 = (center.x + size / 2 - rayOrigin.x) / rayDir.x;
            tNear = std::max(tNear, std::min(t1, t2));
            tFar = std::min(tFar, std::max(t1, t2));

            t1 = (center.y - size / 2 - rayOrigin.y) / rayDir.y;
            t2 = (center.y + size / 2 - rayOrigin.y) / rayDir.y;
            tNear = std::max(tNear, std::min(t1, t2));
            tFar = std::min(tFar, std::max(t1, t2));

            t1 = (center.z - size / 2 - rayOrigin.z) / rayDir.z;
            t2 = (center.z + size / 2 - rayOrigin.z) / rayDir.z;
            tNear = std::max(tNear, std::min(t1, t2));
            tFar = std::min(tFar, std::max(t1, t2));
        }

        // Check if ray hits the cube
        if (tNear <= tFar && tFar >= 0) {
            t = tNear > 0 ? tNear : tFar;
            return true;
        }

        return false;
    }
};

void clearConsole() {
    std::cout << "\x1B[2J\x1B[H";
}

int main() {
    const int numFrames = 100;
    const float angleDelta = 2 * M_PI / numFrames;
    const Vec3 rayOrigin(consoleWidth / 2, consoleHeight / 2, -100); // Position of the "camera"

    Cube cube(Vec3(consoleWidth / 2, consoleHeight / 2, 0), 10);

    for (int frame = 0; frame < numFrames; ++frame) {
        clearConsole();

        float angle = angleDelta * frame;
        Vec3 rayDir(std::cos(angle), std::sin(angle), 0);

        for (int y = 0; y < consoleHeight; ++y) {
            for (int x = 0; x < consoleWidth; ++x) {
                float t;
                if (cube.intersects(rayOrigin, rayDir, t)) {
                    std::cout << '#'; // Print '#' if ray intersects with cube
                } else {
                    std::cout << ' ';
                }
            }
            std::cout << '\n';
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    return 0;
}
