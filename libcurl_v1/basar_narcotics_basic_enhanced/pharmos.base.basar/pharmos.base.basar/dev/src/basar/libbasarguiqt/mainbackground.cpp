//----------------------------------------------------------------------------
/*! \file
 *  \brief  all global statusline for a main window
 *  \author Roland Kiefert
 *  \date   09.12.2005
 */
//----------------------------------------------------------------------------

#include "libbasarguiqt_mainbackground.h"

#pragma warning (push)
#pragma warning(disable: 4251 4481 4512 4800)
#include <QtGui/QPainter>
#pragma warning (pop)

//------------------------------------------------------------------------------
namespace basar
{
    namespace gui
    {
        namespace qt
        {

//------------------------------------------------------------------------------
            /*! \throw no-throw */
            MainBackground::MainBackground()
            {
            }

//------------------------------------------------------------------------------
            /*! \throw no-throw */
            void MainBackground::init( const basar::VarString& rProgrammName,
                                       const basar::VarString& rVersion     ,
                                       const ProductTypeEnum    eProductType,
                                       const ImageEnum          eImage       )
            {
                m_ProductType = eProductType;
                m_Name    = rProgrammName;
                m_Version = rVersion;
                setBackgroundRole( QPalette::Base );

                switch ( eImage )
                {
                    case NEUTRAL:
                        m_Background.load( ":/image/logo_pharmos.png" );
                        break;

                    case PHOENIXGROUP:
                    default:
                        m_Background.load( ":/image/logo_pharmos_phoenix_group_it.png" );
                        break;
                }

                loadProductTypeImage();
            }

//------------------------------------------------------------------------------
            /*! \throw no-throw */
            void MainBackground::init( const VarString& rProgrammName,
                                       const VarString& rVersion     ,
                                       QImage*           pImage       )
            {
                m_ProductType = UNKNOWN;
                m_Background = *pImage;
                m_Name = rProgrammName;
                m_Version = rVersion;
                setBackgroundRole( QPalette::Base );
            }

//------------------------------------------------------------------------------
            /*! \throw no-throw
                \return minimum size */
            QSize MainBackground::minimumSizeHint() const
            {
                return QSize( 224, 196 );
            }

//------------------------------------------------------------------------------
            /*! \throw no-throw
                \return size of Background */
            QSize MainBackground::sizeHint() const
            {
                return QSize( 224, 196 );
            }

//------------------------------------------------------------------------------
            /*! \throw no-throw */
            void MainBackground::paintEvent( QPaintEvent* )
            {

                static const double heightRatio = 0.75;
                static const double widthRatio = 0.75;

                QPainter painter( this );

                QRect bounding;

                int font1 = height() / 14;
                int font2 = height() / 28;

                painter.fillRect( 0, 0, width(), static_cast<int>( height() * heightRatio ), Qt::white );

                painter.drawImage( QRect( static_cast<int>( width() * 0.04 ),
                                          static_cast<int>( height() * heightRatio * 0.45 ),
                                          static_cast<int>( width() * widthRatio ),
                                          static_cast<int>( m_Background.height() * ( width() * widthRatio ) / m_Background.width() ) ),
                                   m_Background );

                if ( m_Name.length() > 0 )
                {
                    QBrush brush( QColor::fromRgb( 87, 86, 86 ) );
                    QFont font;
                    font.setFamily( "Calibri" );
                    font.setWeight( QFont::Light );
                    font.setPointSize( font1 );
                    painter.setFont( font );
                    painter.setPen( brush.color() );
                    painter.drawText( QRect( 10,
                                             static_cast<int>( height() * heightRatio ),
                                             width() - 20,
                                             static_cast<int>( font1 * 2 ) ),
                                      Qt::AlignCenter,
                                      QString::fromLocal8Bit( m_Name.c_str() ), &bounding );
                }

                if ( m_Version.length() > 0 )
                {
                    QRect bounding_small;

                    QBrush brush( QColor::fromRgb( 87, 86, 86 ) );
                    QFont font;
                    font.setFamily( "Calibri" );
                    font.setWeight( QFont::Light );
                    font.setPointSize( font2 );
                    painter.setFont( font );
                    painter.setPen( brush.color() );
                    painter.drawText( QRect( 10,
                                             bounding.bottom(),
                                             width() - 20,
                                             static_cast<int>( font2 * 2 ) ),
                                      Qt::AlignCenter,
                                      QString::fromLocal8Bit( m_Version.c_str() ), &bounding_small );

                }

                if ( m_ProductType != UNKNOWN )
                {
                    painter.drawImage( QRect( bounding.bottomLeft().x() - font1 * 5 / 6,
                                              bounding.bottomLeft().y() - font1 * 2 / 3,
                                              font1 * 3 / 4,
                                              font1 * 3 / 4 ),
                                       m_ProductTypeImage );
                }
            }

//------------------------------------------------------------------------------

            void MainBackground::loadProductTypeImage()
            {
                switch ( m_ProductType )
                {
                    case INBOUND:
                        m_ProductTypeImage.load( ":/image/dot_red.png" );
                        break;

                    case OUTBOUND:
                        m_ProductTypeImage.load( ":/image/dot_blue.png" );
                        break;

                    case WAREHOUSE:
                        m_ProductTypeImage.load( ":/image/dot_yellow.png" );
                        break;

                    case UNKNOWN:
                    default:
                        break;
                }
            }

//------------------------------------------------------------------------------
        }   // namespace qt
    }   // namespace gui
}   // namespace bassar
