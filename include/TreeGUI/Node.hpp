# pragma once

namespace TreeGUI
{

	class Node : public std::enable_shared_from_this<Node> {
	protected:
		/// @brief 見た目の設定 | Appearance settings
		struct config
		{
			static constexpr int32 indent = 30;
			static constexpr int32 padding = 10;
			static constexpr int32 triangleSize = 15;
		};
	public:
		Node(String key, String value = U"", int32 type = 0, std::shared_ptr<Node> parent = nullptr);
		virtual ~Node();
		std::shared_ptr<Node>& operator[](const String& key);

		/// @brief ノードに表示される値を設定する | Set the value displayed in the node
		/// @param value 
		void setDisplayValue(String value);

		/// @brief ノードのタイプを設定する | Set the node type
		/// enum や定数を使ってノードの種類を表すことができます | You can use enums or constants to represent the type of node
		/// @param type 
		void setType(int32 type);

		/// @brief 子ノードを追加する | Add a child node
		/// @param value
		/// @return 追加された子ノード
		std::shared_ptr<Node> addChild(String key, String value = U"", int32 type = 0);

		/// @brief 子ノードを追加する | Add a child node
		/// @param child 
		/// @return 追加された子ノード 
		std::shared_ptr<Node> addChildNode(std::shared_ptr<Node> child);

		/// @brief 祖先ノードを取得する | Get the ancestor node
		/// @param ancestorLevel 自身から見て何世代前の祖先かを指定する | Specify how many generations before yourself the ancestor is
		/// @return 祖先のノード
		std::shared_ptr<Node>p(int32 ancestorLevel);

		/// @brief 親ノードを設定する | Set the parent node
		/// @param parent 
		void setParent(std::shared_ptr<Node> parent);

		/// @brief ノードを開くか閉じるかを設定する | Set whether to open or close the node
		/// @param isOpen
		void setOpen(bool isOpen);

		/// @brief ノードの矩形を設定する | Set the node's rectangle
		/// @param rect
		void setRect(const RectF& rect);

		/// @brief ノードの展開状態を表す三角形を設定する | Set the triangle representing the expansion state of the node
		/// @param triangle 
		void setTriangle(const Triangle& triangle);

		/// @brief テキストの色を設定する | Set the text color
		void setTextColor(const ColorF& color);

		/// @brief 矩形の色を設定する | Set the rectangle color
		void setRectColor(const ColorF& color);

		/// @brief 展開状態を表す三角形の色を設定する | Set the triangle color
		void setTriangleColor(const ColorF& color);

		/// @brief ノード間を繋ぐ線の色を設定する | Set the line color
		void setLineColor(const ColorF& color);

		/// @brief ノードの値を取得する | Get the node's value
		/// @return ノードの値 | Node value
		String getDisplayValue() const;

		/// @brief ノードのタイプを取得する | Get the node type
		/// @return ノードのタイプ | Node type
		int32 getType() const;

		const Array<std::shared_ptr<Node>>& getChildrenArray() const;

		/// @brief 子ノードを取得する | Get the child node
		/// @return 子ノード | Child node
		const HashTable<String, std::shared_ptr<Node>>& getChildrenMap() const;

		/// @brief 親ノードを取得する | Get the parent node
		/// @return 親ノード | Parent node
		const std::shared_ptr<Node> getParent() const;

		/// @brief 自身を含める開いているノードの数を取得する | Get the number of open nodes including yourself
		/// @return 開いているノードの数 | Number of open nodes
		int32 getOpenNodeCount() const;

		/// @brief 子ノードを持っているかを取得する | Get whether you have child nodes
		/// @return 子ノードを持っているか | Whether you have child nodes
		bool hasChildren() const;

		/// @brief ノードが開いているかを取得する | Get whether the node is open
		/// @return ノードが開いているか | Whether the node is open
		bool isOpen() const;

		/// @brief ノードの矩形を取得する | Get the node's rectangle
		/// @return ノードの矩形 | Node rectangle
		RectF getRect() const;

		/// @brief ノードの展開状態を表す三角形を取得する | Get the triangle representing the expansion state of the node
		/// @return 三角形 | Triangle
		Triangle getTriangle() const;

		/// @brief ノードの更新 | Node update
		/// @param pos 展開状態を表す三角形の中心座標 | Center coordinates of the triangle representing the expansion state
		/// @return 
		virtual Vec2 update(Vec2 pos);

		/// @brief ノードの描画 | Node drawing
		virtual void draw() const;

		/// @brief フォルダツリーを作成する | Create a folder tree
		/// @param path フォルダのパス | Folder path
		/// @param displayValue ルートノードの表示名 | Display name of the root node
		/// @return フォルダツリー | Folder tree
		static std::shared_ptr<Node> makeFolderTree(const FilePath& path, const String& displayValue = U"");

		/// @brief　JSONの値を文字列に変換する | Convert JSON value to string
		/// @param json 
		/// @return String
		static String JSONValueToString(const JSON& json);

		/// @brief JSONのツリーを作成する | Create a JSON tree
		/// @param json JSON
		/// @param key ルートノードの表示名 | Display name of the root node
		static std::shared_ptr<Node> makeJSONTree(const JSON& json, const String& key = U"Root");
	protected:
		void updateRect();
		void updateTriangle();
		void drawLines() const;
		void drawChildren() const;
		String m_key;
		String m_displayValue;
		int32 m_type;
		Array<std::shared_ptr<Node>> m_childrenArray;
		HashTable<String, std::shared_ptr<Node>> m_childrenMap;
		std::weak_ptr<Node> m_parent;
		mutable Vec2 m_clickCenterPos;
		mutable Vec2 m_drawLeftCenterPos;
		bool m_isOpen = false;
		RectF m_rect;
		Triangle m_triangle;
		Font m_font{ 20 };
		ColorF m_textColor = Palette::Black;
		ColorF m_rectColor = Palette::White;
		ColorF m_triangleColor = Palette::Gray;
		ColorF m_lineColor = Palette::Black;
	};

}
