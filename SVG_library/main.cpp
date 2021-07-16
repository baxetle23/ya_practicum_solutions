#define _USE_MATH_DEFINES
#include "svg.h" // ?????????? ??????? ?????????? ?????? ???? ??????????? ? ????? svg.h

#include <cmath>

using namespace std::literals;
using namespace svg;


Polyline CreateStar(Point center, double outer_rad, double inner_rad, int num_rays) {
    Polyline polyline;
    for (int i = 0; i <= num_rays; ++i) {
        double angle = 2 * M_PI * (i % num_rays) / num_rays;
        polyline.AddPoint({center.x + outer_rad * sin(angle), center.y - outer_rad * cos(angle)});
        if (i == num_rays) {
            break;
        }
        angle += M_PI / num_rays;
        polyline.AddPoint({center.x + inner_rad * sin(angle), center.y - inner_rad * cos(angle)});
    }
    return polyline;
}

namespace shapes {

class Star : public svg::Drawable {
public:
    void Draw(svg::ObjectContainer& container) const override{
        container.Add(CreateStar(center_, outer_radius_, inner_radius_, num_rays_).SetFillColor(color_fill).SetStrokeColor(color_stroke));
    }
    Star(svg::Point center, double outer_radius, double inner_radius, int num_rays) :
        center_(center),
        outer_radius_(outer_radius),
        inner_radius_(inner_radius),
        num_rays_(num_rays){
    } 
private:
   svg::Point center_;
   double outer_radius_;
   double inner_radius_;
   int num_rays_;
   svg::Color color_fill = "red"s;
   svg::Color color_stroke = "black"s;
}; 

class Snowman : public svg::Drawable {
public:
    void Draw(svg::ObjectContainer& container) const override {
        container.Add(Circle().SetCenter({center_.x, center_.y + 5 * radius_}).SetRadius(2 * radius_).SetFillColor(color_fill).SetStrokeColor(color_stroke));
        container.Add(Circle().SetCenter({center_.x, center_.y + 2 * radius_}).SetRadius(1.5 * radius_).SetFillColor(color_fill).SetStrokeColor(color_stroke));
        container.Add(Circle().SetCenter({center_.x, center_.y}).SetRadius(radius_).SetFillColor(color_fill).SetStrokeColor(color_stroke));
    }
    Snowman(svg::Point center, double raduis) :
        center_(center),
        radius_(raduis) {
    }
private:
   svg::Point center_;
   double radius_;
   svg::Color color_fill = "rgb(240,240,240)"s;
   svg::Color color_stroke = "black"s;
}; 

class Triangle : public svg::Drawable {
public:
    Triangle(svg::Point p1, svg::Point p2, svg::Point p3)
        : p1_(p1)
        , p2_(p2)
        , p3_(p3) {
    }

    // ????????? ????? Draw ?????????? svg::Drawable
    void Draw(svg::ObjectContainer& container) const override {
        container.Add(svg::Polyline().AddPoint(p1_).AddPoint(p2_).AddPoint(p3_).AddPoint(p1_));
    }

private:
    svg::Point p1_, p2_, p3_;
}; 

}

template <typename DrawableIterator>
void DrawPicture(DrawableIterator begin, DrawableIterator end, svg::ObjectContainer& target) {
    for (auto it = begin; it != end; ++it) {
        (*it)->Draw(target);
    }
}

template <typename Container>
void DrawPicture(const Container& container, svg::ObjectContainer& target) {
    using namespace std;
    DrawPicture(begin(container), end(container), target);
}

int main() {
    using namespace svg;
    using namespace shapes;
    using namespace std;

    vector<unique_ptr<svg::Drawable>> picture;
    picture.emplace_back(make_unique<Triangle>(Point{100, 20}, Point{120, 50}, Point{80, 40}));
    picture.emplace_back(make_unique<Star>(Point{50.0, 20.0}, 10.0, 4.0, 5));
    picture.emplace_back(make_unique<Snowman>(Point{30, 20}, 10.0));

    svg::Document doc;
    DrawPicture(picture, doc);

    const Text base_text =  //
        Text()
            .SetFontFamily("Verdana"s)
            .SetFontSize(12)
            .SetPosition({10, 100})
            .SetData("Happy New Year!"s);
    doc.Add(Text{base_text}
                .SetStrokeColor("yellow"s)
                .SetFillColor("yellow"s)
                .SetStrokeLineJoin(StrokeLineJoin::ROUND)
                .SetStrokeLineCap(StrokeLineCap::ROUND)
                .SetStrokeWidth(3));
    doc.Add(Text{base_text}.SetFillColor("red"s));

    doc.Render(cout);
} 

