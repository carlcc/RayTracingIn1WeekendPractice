#version 330 core
out vec4 FragColor;

// ======== basic information ===========
uniform int viewPortWidth;
uniform int viewPortHeight;

struct Camera {
    vec3 origin;
    vec3 lowerLeftCorner;
    vec3 horizontal;
    vec3 vertical;
    float lensRadius;
    vec3 right;
    vec3 up;
    vec3 back;
};

#define MAT_GLASS 0
#define MAT_METAL 1
#define MAT_DIFFUSE 3
struct Material {
    int type;
    vec3 color;
    float fractionRate;
};

struct Sphere {
    vec3 center;
    float radius;
    int material;
};

struct Ray {
    vec3 origin;
    vec3 direction;
};

struct HitRecord {
    float t;
    vec3 p;
    vec3 normal;
    int material;
};

#define N_SPHERES 100
// ======== the scene ===================
uniform Camera gCamera;
uniform Sphere gWorld[N_SPHERES];
uniform Material gMaterials[N_SPHERES];
uniform int gNSpheres;

// ======== basic tools ==================
int RandSeed;

void Srand(vec2 x)
{
    int n = int(x.x * 40.0 + x.y * 6400.0);
    RandSeed = n;
}


float RandomFloat()
{
    RandSeed = (RandSeed << 13) ^ RandSeed;
    return 1.0 - float( (RandSeed * (RandSeed * RandSeed * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0;
}

vec3 RandomUnitVectorInBall()
{
//    while (true) {
        vec3 p = vec3(RandomFloat(), RandomFloat(), RandomFloat());
//        if (p.x*p.x + p.y*p.y + p.z*p.z > 1)
//            continue;
        return normalize(p);
//    }
}

Ray Camera_GetRay()
{
    Ray r;

    float u = float(int(gl_FragCoord.x)) / viewPortWidth;
    float v = float(int(gl_FragCoord.y)) / viewPortHeight;

    r.origin = gCamera.origin;
    r.direction = gCamera.lowerLeftCorner + u*gCamera.horizontal + v*gCamera.vertical;

    return r;
}

vec3 Ray_GetDirection(in Ray r)
{
    return r.direction;
}

vec3 Ray_GetPointAt(in Ray r, float t)
{
    return r.origin + r.direction * t;
}

bool Sphere_Hit(in Sphere obj, float tMin, float tMax, in Ray r, out HitRecord rec)
{
    vec3 oc = r.origin - obj.center;
    float a = dot(r.direction, r.direction);
    float b = dot(oc, r.direction);
    float c = dot(oc, oc) - obj.radius*obj.radius;
    float discriminant = b*b - a*c;

    if (discriminant > 0) {
        float tmp = (-b - sqrt(b*b-a*c)) / a;
        if (tmp < tMax && tmp > tMin) {
            rec.t = tmp;
            rec.p = Ray_GetPointAt(r, rec.t);
            rec.normal = (rec.p - obj.center) / obj.radius;
            rec.material = obj.material;
            return true;
        }
        tmp = (-b + sqrt(b*b-a*c)) / a;
        if (tmp < tMax && tmp > tMin) {
            rec.t = tmp;
            rec.p = Ray_GetPointAt(r, rec.t);
            rec.normal = (rec.p - obj.center) / obj.radius;
            rec.material = obj.material;
            return true;
        }
    }
    return false;
}

//
bool WorldList_Hit(in Ray r, float tMin, float tMax, out HitRecord rec)
{
    bool hasHit = false;
    float closestSoFar = tMax;
    for (int i = 0; i < gNSpheres; ++i) {
        HitRecord tmpRec;
        if (Sphere_Hit(gWorld[i], tMin, closestSoFar, r, tmpRec)) {
            hasHit = true;
            closestSoFar = tmpRec.t;
            rec = tmpRec;
        }
    }

    return hasHit;
}

bool Material_Scatter(int materialIndex, in Ray r, in HitRecord rec, out vec3 attenuation, out Ray scattered)
{
    if (gMaterials[materialIndex].type == MAT_METAL) {
        scattered.origin = rec.p;
        scattered.direction = reflect(r.direction, rec.normal);
        attenuation = gMaterials[materialIndex].color;
    } else if (gMaterials[materialIndex].type == MAT_DIFFUSE) {
        scattered.origin = rec.p;
        scattered.direction = rec.normal + RandomUnitVectorInBall();//reflect(r.direction, rec.normal);
        attenuation = gMaterials[materialIndex].color;
    } else {

    }

    return true;
}

vec3 getColor(in Ray r, int depth)
{
    HitRecord rec;
    Ray ray = r;
    float tMin = 0.001f;
    float tMax = 9999999.0f;
    vec3 attenuation = vec3(1.0f);
    while (depth-- > 0) {
        if (WorldList_Hit(ray, tMin, tMax, rec)) {
            vec3 atten;
            Ray scattered;
            if (Material_Scatter(rec.material, ray, rec, atten, scattered)) {
                attenuation *= atten;
                ray = scattered;
                continue;
            }
//            return vec3(RandomFloat(), RandomFloat(), RandomFloat());
            return vec3(.0f, 0.0f, 0.0f);
        } else {
            float y = normalize(r.direction).y;
            float t = 0.5f * (y + 1);
            vec3 skyColor = (1.0 - t) * vec3(1.f, 1.0f, 1.0f) + t*vec3(0.5f, 0.7f, 1.0f);
            return attenuation * skyColor;
        }
    }
    return vec3(0);
}

void main()
{
    Srand(gl_FragCoord.xy);

    vec3 color = vec3(0);

    for (int i = 0; i < 1; ++i) {
        Ray ray = Camera_GetRay();
        vec3 rayDir = normalize(Ray_GetDirection(ray));
        color += getColor(ray, 15);
    }
    color /= 1;
    color = vec3(sqrt(color.x),sqrt(color.y),sqrt(color.z));

    FragColor = vec4(color, 1.0f);
}