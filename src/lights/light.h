#pragma once

#include "materials/material.h"
#include "render/prim.h"
#include "utils/photon.h"
#include "utils/vec.h"
#include <memory>

class Light
{
  public:
    enum Type : uint32_t
    {
        BASE          = 1,
        INTERSECTABLE = 1 << 1,
        POINT         = 1 << 2,
        RECTANGLE     = 1 << 3
    };

    struct Sample
    {
        Vec2  objP; // surface
        float invPDF;
        Vec3  wP;
    };

    Light(std::shared_ptr<LightMaterial> mat) : m_mat(mat), m_flags(0) {}

    virtual Sample       sample()                      = 0;
    virtual Vec3         evalLe(Sample sample, Vec3 p) = 0;
    virtual Vec3         getTotalPower()               = 0;
    virtual Photon       shootPhoton(float weight)     = 0;
    virtual Intersection intersect(const Ray& ray) { return Intersection::NoHit; }

    uint32_t getFlags() const { return m_flags; }

    const std::shared_ptr<LightMaterial> getMat() const { return m_mat; }

    Type getType() const { return m_type; }

  protected:
    const static Type              m_type = BASE;
    std::shared_ptr<LightMaterial> m_mat;
    uint32_t                       m_flags;
};

class PointLight : public Light
{
  public:
    PointLight(std::shared_ptr<LightMaterial> mat, Vec3 pos) : Light(mat), m_pos(pos) {}

    virtual Sample sample() override;
    virtual Vec3   evalLe(Sample sample, Vec3 p) override;
    virtual Vec3   getTotalPower() override;
    virtual Photon shootPhoton(float weight) override;

  private:
    Vec3 m_pos;
};

class IntersectableLight : public Light
{
  public:
    IntersectableLight(std::shared_ptr<LightMaterial> mat) : Light(mat)
    {
        m_flags |= INTERSECTABLE;
    }

    virtual std::shared_ptr<Prim> getPrim()                 = 0;
    virtual Photon                shootPhoton(float weight) = 0;

  protected:
    const static Type m_type = INTERSECTABLE;
};

class RectangleLight : public IntersectableLight
{
  public:
    RectangleLight(std::shared_ptr<LightMaterial> mat, std::shared_ptr<Rectangle> rect)
        : IntersectableLight(mat), m_rect(rect)
    {
    }

    std::shared_ptr<Prim> getPrim() override;
    virtual Sample        sample() override;
    virtual Vec3          evalLe(Sample sample, Vec3 p) override;
    virtual Intersection  intersect(const Ray& ray) override;
    virtual Vec3          getTotalPower() override;
    virtual Photon        shootPhoton(float weight) override;

  protected:
    const static Type m_type = RECTANGLE;

  private:
    std::shared_ptr<Rectangle> m_rect;
};
