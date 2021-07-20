#include "svg.h"

namespace svg {

using namespace std::literals;

void Object::Render(const RenderContext& context) const {
    context.RenderIndent();
    RenderObject(context);
    context.out << std::endl;
}


// ---------------RGB--------------

Rgb::Rgb(size_t R, size_t G, size_t B) :
    red(R),
    green(G),
    blue(B) {
}

Rgba::Rgba(size_t R, size_t G, size_t B, double opacity) :
    red(R),
    green(G),
    blue(B),
    opacity(opacity){
}

void PrintColor::operator()(std::monostate) const{
    out << "none"s;
}
void PrintColor::operator()(const std::string& str) const {
    out << str;
}
void PrintColor::operator()(Rgb rgb) const {
    out << "rgb("s << std::to_string(rgb.red)
        << ","s << std::to_string(rgb.green)
        << ","s << std::to_string(rgb.blue) << ")"s;
}
void PrintColor::operator()(Rgba rgba)  const{
    out << "rgba("s << std::to_string(rgba.red)
        << ","s << std::to_string(rgba.green)
        << ","s << std::to_string(rgba.blue)
        << ","s << rgba.opacity << ")"s;
}

// -----------PathProps-------------



// ---------- Circle ------------------

Circle& Circle::SetCenter(Point center)  {
    center_ = center;
    return *this;
}

Circle& Circle::SetRadius(double radius)  {
    radius_ = radius;
    return *this;
}

void Circle::RenderObject(const RenderContext& context) const {
    auto& out = context.out;
    out << "<circle cx=\""sv << center_.x << "\" cy=\""sv << center_.y << "\" "sv;
    out << "r=\""sv << radius_ << "\" "sv;
    RenderAttrs(out);
    out << "/>"sv;
}
// --------------TEXT_-----------------

void Text::RenderObject(const RenderContext& context) const {
    auto& out = context.out;
    out << "<text x=\""sv << pos_.x << "\" y=\""sv << pos_.y << "\" "sv;
    out << "dx=\""sv << offset_.x << "\" dy=\""sv << offset_.y << "\" "sv;
    out << "font-size=\""sv << size_ << "\""sv;
    if (font_family_) {
        out << " font-family=\""sv << *font_family_ << "\""sv;
    }
    if (font_weight_) {
        out << " font-weight=\""sv << *font_weight_ << "\""sv;
    }
    RenderAttrs(out);
    out << ">"sv << data_ << "</text>"sv;
}

Text& Text::SetPosition(Point pos) {
    pos_ = pos;
    return *this;
}

Text& Text::SetOffset(Point offset) {
    offset_ = offset;
    return *this;
}

Text& Text::SetFontSize(uint32_t size = 1) {
    size_ = size;
    return *this;
}

Text& Text::SetFontFamily(std::string font_family) {
    font_family_ = font_family;
    return *this;
}

Text& Text::SetFontWeight(std::string font_weight) {
    font_weight_ = font_weight;
    return *this;
}

Text& Text::SetData(std::string data) {
    for_each(data.begin(), data.end(), [&](auto & s) {
        if (s == '\"') {
            (data_) += "&quot;"s;
        } else if (s == '\'') {
            (data_) += "&apos;"s;
        } else if (s == '<') {
            (data_) += "&lt;"s; 
        } else if (s == '>') {
            (data_) += "&rt;"s;
        } else if (s == '&') {
            (data_) += "&amp;"s;
        } else {
            (data_) += s;
        }
    });
    return *this;
}

//--------------Polyline-------------

void Polyline::RenderObject(const RenderContext& context) const {
    auto& out = context.out;
    out << "<polyline points=\""sv;
    bool space = false;
    for (auto& point : points_) {
        if (!space) {
            space = true;
            out << point.x << ","sv << point.y;
            continue;
        }
        out << " "sv << point.x << ","sv << point.y; 
    }
    out << "\"";
    RenderAttrs(out);
    out << "/>"sv;
}

Polyline& Polyline::AddPoint(Point point) {
    points_.push_back(point);
    return *this;
}

//---------------Document-------------

void Document::AddPtr(std::unique_ptr<Object>&& obj) {
    objects_.emplace_back(std::move(obj));
}

void Document::Render(std::ostream& out) const {
    std::cout << "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>"sv << std::endl;
    std::cout << "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\">"sv << std::endl;
    RenderContext ctx(out, 2, 2);
    for (auto& object : objects_) {
        object->Render(ctx);
    }
    std::cout << "</svg>"sv;
}
}  // namespace svg


std::ostream& operator<<( std::ostream& stm, const svg::StrokeLineCap& clr){
    using namespace std::literals;
    switch(clr)
    {
        case svg::StrokeLineCap::BUTT   : {stm << "butt"sv ; break;}
        case svg::StrokeLineCap::ROUND  : {stm << "round"sv; break;}
        case svg::StrokeLineCap::SQUARE : {stm << "square"sv; break;}
    }
    return stm;
}

std::ostream& operator<<(std::ostream& stm, const svg::StrokeLineJoin& clr) {
    using namespace std::literals;
    switch(clr)
    {
        case svg::StrokeLineJoin::ARCS       : {stm << "arcs"sv; break;}
        case svg::StrokeLineJoin::BEVEL      : {stm << "bevel"sv; break;}
        case svg::StrokeLineJoin::MITER      : {stm << "miter"sv; break;}
        case svg::StrokeLineJoin::MITER_CLIP : {stm << "miter-clip"sv; break;}
        case svg::StrokeLineJoin::ROUND      : {stm << "round"sv; break;}
    }
    return stm;
}

std::ostream& operator<<(std::ostream& stm, const svg::Color& color) {
    std::visit(svg::PrintColor{stm}, color);
    return stm;
}
