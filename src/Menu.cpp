#include <slib/Menu.h>

namespace slib {

	//////////////////// Item /////////////////////
	Menu::Item::~Item( ) {
	}

	///////////////////// Frame /////////////////////
	Menu::Frame::Frame( const vec2f & _dimensions ) : dimensions( _dimensions ), activatediter( items.end( ) ) { }

	Menu::Frame::~Frame( ) {
		for( std::list< std::pair< vec2f, Item * > >::iterator iter = items.begin( ); iter != items.end( ); iter++ ) {
			delete iter->second;
		}
	}

	void Menu::Frame::Add( const vec2f & position, Item * Item ) {
		items.push_back( std::make_pair( position, Item ) );
	}

	void Menu::Frame::SetActivated( const Menu::Item * item ) {
		activatediter = items.end( );

		for( std::list< std::pair< vec2f, Item * > >::iterator iter = items.begin( ); iter != items.end( ); iter++ ) {
			std::pair< vec2f, Item * > & itempair = *iter;
			
			if( itempair.second == item ) {
				activatediter = iter;
			}
		}
	}

	bool Menu::Frame::Within( const vec2f & position ) {
		return position[ 0 ] >= 0.0f && position[ 1 ] >= 0.0f && position[ 0 ] < dimensions[ 0 ] && position[ 1 ] < dimensions[ 1 ];
	}

	void Menu::Frame::MouseClick( const vec2f & position ) {
		for( std::list< std::pair< vec2f, Item * > >::iterator iter = items.begin( ); iter != items.end( ); iter++ ) {
			std::pair< vec2f, Item * > & item = *iter;

			if( item.second->Within( position - item.first ) ) {
				item.second->MouseClick( position - item.first );
				activatediter = iter;
				return;
			}
		}

		activatediter = items.end( );
	}

	bool Menu::Frame::KeyPress( int key ) {
		bool captured = false;
		if( activatediter != items.end( ) ) {
			captured = activatediter->second->KeyPress( key );
		}

		if( !captured && key == 9 ) {
			if( activatediter != items.end( ) ) {
				activatediter++;
				if( activatediter == items.end( ) ) {
					activatediter = items.begin( );
				}
			}
		}

		return captured;
	}

	void Menu::Frame::Render( Renderer * renderer, const vec2f & position, float time, bool activated ) const {
		renderer->drawBox( position, dimensions, Vector4( 0.4f, 0.4f, 0.7f, 0.5f ) );

		for( std::list< std::pair< vec2f, Item * > >::const_iterator iter = items.begin( ); iter != items.end( ); iter++ ) {
			const std::pair< vec2f, Item * > & item = *iter;

			item.second->Render( renderer, position + item.first, time, activatediter == iter );
		}
	}

	//////////////////// Textbox ////////////////////////
	//Menu::Textbox::Textbox( const vec2f & _dimensions, void (*_enterfunc)( ), void (*_tabfunc)( ) ) : dimensions( _dimensions ), enterfunc( _enterfunc ), tabfunc( _tabfunc ) { }
	Menu::Textbox::Textbox( const vec2f & _dimensions, const std::map< int, void (*)( ) > & _keyfuncs ) : dimensions( _dimensions ), keyfuncs( _keyfuncs ) { }

	std::string & Menu::Textbox::GetText( ) {
		return text;
	}

	bool Menu::Textbox::Within( const vec2f & position ) {
		return position[ 0 ] >= 0.0f && position[ 1 ] >= 0.0f && position[ 0 ] < dimensions[ 0 ] && position[ 1 ] < dimensions[ 1 ];
	}

	void Menu::Textbox::MouseClick( const vec2f & position ) {
	}

	bool Menu::Textbox::KeyPress( int key ) {
		if( keyfuncs.find( key ) != keyfuncs.end( ) ) {
			keyfuncs[ key ]( );
			return true;
		} else if( key == 8 ) {
			if( text.size( ) > 0 ) {
				text.erase( text.size( ) - 1 );
			}
		} else {
			text += key;
		}
		return false;
	}

	void Menu::Textbox::Render( Renderer * renderer, const vec2f & position, float time, bool activated ) const {
		renderer->drawBox( position, dimensions, activated ? Vector4( 0.0f, 0.0f, 0.0f, 0.85f ) : Vector4( 0.0f, 0.0f, 0.0f, 0.45f ) );

		std::string rendertext = text;
		if( activated && (int)( time * 2.0f ) % 2 == 0 ) {
			rendertext += "_";
		}
		renderer->drawString( rendertext, position, dimensions );
	}

	//////////////////// MultiTextbox ////////////////////////
	Menu::MultiTextbox::MultiTextbox( const vec2f & _dimensions ) : startrow( 0 ), dimensions( _dimensions ) { }

	void Menu::MultiTextbox::AddText( const std::string & text ) {
		textlist.push_back( text );
		startrow = 0;
	}

	std::string Menu::MultiTextbox::GetText( size_t position ) const {
		std::list< std::string >::const_iterator iter = textlist.begin( );
		for( size_t i = 0; i < position && iter != textlist.end( ); ++i, ++iter );
		return iter != textlist.end( ) ? *iter : std::string( );
	}

	size_t Menu::MultiTextbox::NumLines( ) const {
		return textlist.size( );
	}

	bool Menu::MultiTextbox::Within( const vec2f & position ) {
		return position[ 0 ] >= 0.0f && position[ 1 ] >= 0.0f && position[ 0 ] < dimensions[ 0 ] && position[ 1 ] < dimensions[ 1 ];
	}

	void Menu::MultiTextbox::MouseClick( const vec2f & position ) {
	}

	bool Menu::MultiTextbox::KeyPress( int key ) {
		if( key == 256 ) {
			startrow++;
			if( startrow >= textlist.size( ) ) {
				startrow = textlist.size( ) - 1;
			}
		} else if( key == 257 ) {
			startrow--;
			if( startrow < 0 ) {
				startrow = 0;
			}
		} else if( key == 258 ) {
			startrow += 8;
			if( startrow >= textlist.size( ) ) {
				startrow = textlist.size( ) - 1;
			}
		} else if( key == 259 ) {
			startrow -= 8;
			if( startrow < 0 ) {
				startrow = 0;
			}
		}

		return false;
	}

	void Menu::MultiTextbox::Render( Renderer * renderer, const vec2f & position, float time, bool activated ) const {
		renderer->drawBox( position, dimensions, activated ? Vector4( 0.0f, 0.0f, 0.0f, 0.85f ) : Vector4( 0.0f, 0.0f, 0.0f, 0.45f ) );

		std::list< std::string >::const_reverse_iterator iter = textlist.rbegin( );
		for( int i = 0; i < startrow && iter != textlist.rend( ); ++iter, ++i );
		for( float y = 0.0f; y <= dimensions[ 1 ] && iter != textlist.rend( ); y += 1.0f / 24.0f, ++iter ) {
			renderer->drawString( *iter, Vector2( position[ 0 ], position[ 1 ] + y ), Vector2( dimensions[ 0 ], 1.0f / 24.0f ) );
		}
		//renderer->drawString( text, position, dimensions );
	}

	/////////////////////// Label /////////////////////////
	Menu::Label::Label( const std::string & _text, const vec2f & _dimensions ) : text( _text ), dimensions( _dimensions ) { }

	std::string & Menu::Label::GetText( ) {
		return text;
	}

	bool Menu::Label::Within( const vec2f & position ) {
		return position[ 0 ] >= 0.0f && position[ 1 ] >= 0.0f && position[ 0 ] < dimensions[ 0 ] && position[ 1 ] < dimensions[ 1 ];
	}

	void Menu::Label::MouseClick( const vec2f & position ) {
	}

	bool Menu::Label::KeyPress( int key ) {
		return false;
	}

	void Menu::Label::Render( Renderer * renderer, const vec2f & position, float time, bool activated ) const {
		renderer->drawBox( position, dimensions, Vector4( 0.0f, 0.0f, 0.0f, 0.5f ) );
		renderer->drawString( text, position, dimensions );
	}

	//////////////////// Button /////////////////////////
	Menu::Button::Button( const std::string & _text, const vec2f & _dimensions, void (*_buttonfunc)( ) ) : text( _text ), dimensions( _dimensions ), buttonfunc( _buttonfunc ) { }

	bool Menu::Button::Within( const vec2f & position ) {
		return position[ 0 ] >= 0.0f && position[ 1 ] >= 0.0f && position[ 0 ] < dimensions[ 0 ] && position[ 1 ] < dimensions[ 1 ];
	}

	void Menu::Button::MouseClick( const vec2f & position ) {
		buttonfunc( );
	}

	bool Menu::Button::KeyPress( int key ) {
		if( key == 13 ) {
			buttonfunc( );
			return true;
		}

		return false;
	}

	void Menu::Button::Render( Renderer * renderer, const vec2f & position, float time, bool activated ) const {
		renderer->drawBox( position, dimensions, activated ? Vector4( 1.0f, 0.0f, 0.0f, 0.75f ) : Vector4( 0.0f, 0.0f, 0.0f, 0.5f ) );
		renderer->drawString( text, position, dimensions );
	}

}

