#ifndef VERTEX_H
#define VERTEX_H

#include <QVector3D>
#include <QVector2D>

class Vertex
{
public:
    // Constructors
    Q_DECL_CONSTEXPR Vertex();
    Q_DECL_CONSTEXPR explicit Vertex(const QVector3D &position);
    Q_DECL_CONSTEXPR Vertex(const QVector3D &position, const QVector3D &color);
    Q_DECL_CONSTEXPR Vertex(
            const QVector3D &position,
            const QVector3D &color,
            const QVector2D &texCoords);
    // Accessors / Mutators
    Q_DECL_CONSTEXPR const QVector3D& position() const;
    Q_DECL_CONSTEXPR const QVector3D& color() const;
    Q_DECL_CONSTEXPR const QVector2D& texCoords() const;
    void setPosition(const QVector3D& position);
    void setColor(const QVector3D& color);
    void setTexCoords(const QVector2D& texCoords);

    // OpenGL Helpers
    static const int PositionTupleSize = 3;
    static const int ColorTupleSize = 3;
    static const int TexCoordsTupleSize = 2;
    static Q_DECL_CONSTEXPR int positionOffset();
    static Q_DECL_CONSTEXPR int colorOffset();
    static Q_DECL_CONSTEXPR int texCoordsOffset();
    static Q_DECL_CONSTEXPR int stride();

private:
    QVector3D m_position;
    QVector3D m_color;
    QVector2D m_texCoords;
};

/*******************************************************************************
 * Inline Implementation
 ******************************************************************************/

// Note: Q_MOVABLE_TYPE means it can be memcpy'd.
Q_DECLARE_TYPEINFO(Vertex, Q_MOVABLE_TYPE);

// Constructors
Q_DECL_CONSTEXPR inline Vertex::Vertex() {}
Q_DECL_CONSTEXPR inline Vertex::Vertex(const QVector3D &position)
    : m_position(position)
{

}
Q_DECL_CONSTEXPR inline Vertex::Vertex(
        const QVector3D &position,
        const QVector3D &color)
    : m_position(position), m_color(color)
{

}

Q_DECL_CONSTEXPR inline Vertex::Vertex(
        const QVector3D &position,
        const QVector3D &color,
        const QVector2D &texCoords)
    : m_position(position), m_color(color), m_texCoords(texCoords)
{

}

// Accessors / Mutators
Q_DECL_CONSTEXPR inline const QVector3D& Vertex::position() const
{
    return m_position;
}

Q_DECL_CONSTEXPR inline const QVector3D& Vertex::color() const
{
    return m_color;
}

Q_DECL_CONSTEXPR inline const QVector2D &Vertex::texCoords() const
{
    return m_texCoords;
}

void inline Vertex::setPosition(const QVector3D& position)
{
    m_position = position;
}

void inline Vertex::setColor(const QVector3D& color)
{
    m_color = color;
}

void inline Vertex::setTexCoords(const QVector2D &texCoords)
{
    m_texCoords = texCoords;
}

// OpenGL Helpers
Q_DECL_CONSTEXPR inline int Vertex::positionOffset()
{
    return offsetof(Vertex, m_position);
}

Q_DECL_CONSTEXPR inline int Vertex::colorOffset()
{
    return offsetof(Vertex, m_color);
}

Q_DECL_CONSTEXPR inline int Vertex::texCoordsOffset()
{
    return offsetof(Vertex, m_texCoords);
}

Q_DECL_CONSTEXPR inline int Vertex::stride()
{
    return sizeof(Vertex);
}

#endif // VERTEX_H
