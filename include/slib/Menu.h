#ifndef SLIB_MENU_H
#define SLIB_MENU_H

#include <list>
#include <string>
#include <map>

#include <slib/Matrix.hpp>

namespace slib {

	namespace Menu {

		class Renderer {
		public:
			virtual void drawBox( const vec2f & position, const vec2f & dimensions, const vec4f & color ) = 0;
			virtual void drawString( const std::string & text, const vec2f & position, const vec2f & dimensions ) = 0;
		};

		class Item {
		public:
			virtual ~Item( );
			virtual bool Within( const vec2f & position ) = 0;
			virtual void MouseClick( const vec2f & position ) = 0;
			virtual bool KeyPress( int key ) = 0;
			virtual void Render( Renderer * renderer, const vec2f & position, float time, bool activated ) const = 0;
		private:
		};

		class Frame : public Item {
		public:
			Frame( const vec2f & _dimensions );
			~Frame( );
			void Add( const vec2f & position, Item * Item );
			void SetActivated( const Item * item );
			bool Within( const vec2f & position );
			void MouseClick( const vec2f & position );
			bool KeyPress( int key );
			void Render( Renderer * renderer, const vec2f & position, float time, bool activated ) const;
		private:
			vec2f dimensions;
			std::list< std::pair< vec2f, Item * > > items;
			std::list< std::pair< vec2f, Item * > >::iterator activatediter;
		};

		class Textbox : public Item {
		public:
			Textbox( const vec2f & _dimensions, const std::map< int, void (*)( ) > & _keyfuncs = std::map< int, void (*)( ) >( ) );
			std::string & GetText( );
			bool Within( const vec2f & position );
			void MouseClick( const vec2f & position );
			bool KeyPress( int key );
			void Render( Renderer * renderer, const vec2f & position, float time, bool activated ) const;
		private:
			vec2f dimensions;
			std::string text;
			std::map< int, void (*)( ) > keyfuncs;
		};

		class MultiTextbox : public Item {
		public:
			MultiTextbox( const vec2f & _dimensions );
			void AddText( const std::string & text );
			std::string GetText( size_t position ) const;
			size_t NumLines( ) const;
			bool Within( const vec2f & position );
			void MouseClick( const vec2f & position );
			bool KeyPress( int key );
			void Render( Renderer * renderer, const vec2f & position, float time, bool activated ) const;
		private:
			int startrow;
			vec2f dimensions;
			std::list< std::string > textlist;
		};

		class Label : public Item {
		public:
			Label( const std::string & _text, const vec2f & _dimensions );
			std::string & GetText( );
			bool Within( const vec2f & position );
			void MouseClick( const vec2f & position );
			bool KeyPress( int key );
			void Render( Renderer * renderer, const vec2f & position, float time, bool activated ) const;
		private:
			vec2f dimensions;
			std::string text;
		};

		class Button : public Item {
		public:
			Button( const std::string & _text, const vec2f & _dimensions, void (*_buttonfunc)( ) );
			bool Within( const vec2f & position );
			void MouseClick( const vec2f & position );
			bool KeyPress( int key );
			void Render( Renderer * renderer, const vec2f & position, float time, bool activated ) const;
		private:
			vec2f dimensions;
			std::string text;
			void (*buttonfunc)( );
		};

	}

}

#endif