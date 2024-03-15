# include "TreeGUI/Node.hpp"

namespace TreeGUI
{
	Node::Node(String key, String value, int32 type, std::shared_ptr<Node> parent)
		: m_key(key), m_displayValue(value.empty() ? key : value), m_type(type), m_parent(parent) {}


	Node::~Node()
	{
	}

	std::shared_ptr<Node>& Node::operator[](const String& key)
	{
		auto it = m_childrenMap.find(key);
		if (it != m_childrenMap.end())
		{
			return it->second;
		}
		else
		{
			throw Error{ U"Key `{}` not found"_fmt(key) };
		}
	}

	void Node::setDisplayValue(String value)
	{
		m_displayValue = value;
	}

	void Node::setType(int32 type)
	{
		m_type = type;
	}

	std::shared_ptr<Node> Node::addChild(String key, String value, int32 type)
	{
		auto child = std::make_shared<Node>(key, value, type, shared_from_this());
		m_childrenArray.push_back(child);
		m_childrenMap[child->m_key] = child;
		return child;
	}

	std::shared_ptr<Node> Node::p(int32 ancestorLevel)
	{
		std::shared_ptr<Node> returnNode = shared_from_this();
		int32 absoluteLevel = std::abs(ancestorLevel);
		for (int i = 0; i < absoluteLevel; ++i)
		{
			if (auto parent = returnNode->m_parent.lock())
			{
				returnNode = parent;
			}
			else
			{
				throw std::runtime_error("No ancestor exists at the specified level.");
			}
		}
		return returnNode;
	}

	std::shared_ptr<Node> Node::addChildNode(std::shared_ptr<Node> child)
	{
		// Reserve space for the children list
		if (m_childrenMap.empty())
		{
			m_childrenMap.reserve(10);  // Adjust this value based on your needs
		}

		child->setParent(shared_from_this());
		m_childrenArray.push_back(child);
		m_childrenMap[child->m_key] = child;
		return child;
	}

	void Node::setParent(std::shared_ptr<Node> parent)
	{
		m_parent = parent;
	}

	void Node::setOpen(bool isOpen)
	{
		m_isOpen = isOpen;
	}

	void Node::setRect(const RectF& rect)
	{
		m_rect = rect;
	}

	void Node::setTriangle(const Triangle& triangle)
	{
		m_triangle = triangle;
	}

	void Node::setTextColor(const ColorF& color)
	{
		m_textColor = color;
	}

	void Node::setRectColor(const ColorF& color)
	{
		m_rectColor = color;
	}

	void Node::setTriangleColor(const ColorF& color)
	{
		m_triangleColor = color;
	}

	void Node::setLineColor(const ColorF& color)
	{
		m_lineColor = color;
	}

	String Node::getDisplayValue() const
	{
		return m_displayValue;
	}

	int32 Node::getType() const
	{
		return m_type;
	}

	const Array<std::shared_ptr<Node>>& Node::getChildrenArray() const
	{
		return m_childrenArray;
	}

	const HashTable<String, std::shared_ptr<Node>>& Node::getChildrenMap() const
	{
		return m_childrenMap;
	}

	const std::shared_ptr<Node> Node::getParent() const
	{
		return m_parent.lock();
	}

	int32 Node::getOpenNodeCount() const
	{
		int32 count = 1;
		if (isOpen())
		{
			for (auto& child : getChildrenArray())
			{
				count += child->getOpenNodeCount();
			}
		}
		return count;
	}

	bool Node::hasChildren() const
	{
		return not m_childrenArray.isEmpty();
	}

	bool Node::isOpen() const
	{
		return m_isOpen;
	}

	RectF Node::getRect() const
	{
		return m_rect;
	}

	Triangle Node::getTriangle() const
	{
		return m_triangle;
	}

	Vec2 Node::update(Vec2 pos)
	{
		m_clickCenterPos = pos;
		m_drawLeftCenterPos = m_clickCenterPos.movedBy(25, 0);

		updateRect();
		updateTriangle();

		pos.y += m_rect.h + Node::config::padding;

		if (isOpen())
		{
			double originalX = pos.x;
			for (auto& child : getChildrenArray())
			{
				pos = child->update(Vec2(originalX + Node::config::indent, pos.y));
			}
		}
		return pos;
	}

	void Node::draw() const
	{
		m_rect.draw(Palette::White).drawFrame(1.0, m_rectColor);
		m_font(getDisplayValue()).draw(Arg::leftCenter = m_drawLeftCenterPos, m_textColor);
		m_triangle.draw(m_triangleColor);
		if (isOpen())
		{
			drawLines();
			drawChildren();
		}
	}

	void Node::updateRect()
	{
		setRect(m_font(m_displayValue).draw(Arg::leftCenter = m_drawLeftCenterPos).stretched(10, 0));
		if (not hasChildren()) return;
		if (getRect().mouseOver())
		{
			Cursor::RequestStyle(CursorStyle::Hand);
		}
		if (getRect().leftClicked())
		{
			setOpen(!isOpen());
		}
	}

	std::shared_ptr<Node> Node::makeFolderTree(const FilePath& path, const String& displayValue)
	{
		auto node = std::make_shared<Node>(displayValue.isEmpty() ? FileSystem::FileName(path) : displayValue);
    
		if (FileSystem::IsDirectory(path))
		{
			for (const auto& childPath : FileSystem::DirectoryContents(path, Recursive::No))
			{
				if (FileSystem::IsDirectory(childPath))
				{
					auto childNode = makeFolderTree(childPath);
					node->addChildNode(childNode);
				}
				else
				{
					node->addChild(FileSystem::FileName(childPath), FileSystem::FileName(childPath), static_cast<int32>(FileSystem::IsDirectory(childPath)));
				}
			}
		}
		return node;
	}

	String Node::JSONValueToString(const JSON& json)
	{
		switch (json.getType())
		{
		case JSONValueType::String:
			return json.getString();
		case JSONValueType::Number:
			return ToString(json.get<double>());
		case JSONValueType::Bool:
			return json.get<bool>() ? U"true" : U"false";
		default:
			return U"";
		}
	}

	std::shared_ptr<Node> Node::makeJSONTree(const JSON& json, const String& key)
	{
		// Create a new KeyValueNode only when it is needed
		std::shared_ptr<Node> node;

		if (json.isObject())
		{
			for (const auto& [objectKey, objectValue] : json)
			{
				// Create the node here
				if (!node)
				{
					node = std::make_shared<Node>(key, key + U" : ", static_cast<int32>(objectValue.getType()));
				}

				auto childNode = makeJSONTree(objectValue, objectKey);
				node->addChildNode(childNode);
			}
		}
		else if (json.isArray())
		{
			int index = 0;
			for (const auto& element : json.arrayView())
			{
				// Create the node here
				if (!node)
				{
					node = std::make_shared<Node>(key, key + U" : ", static_cast<int32>(element.getType()));
				}

				String childKey = U"[{}]"_fmt(index++);
				auto childNode = makeJSONTree(element, childKey);
				node->addChildNode(childNode);
			}
		}

		// If the node was not created in the loop, create it here
		if (!node)
		{
			node = std::make_shared<Node>(key, key + U" : " + JSONValueToString(json), static_cast<int32>(json.getType()));
		}
		return node;
	}


	void Node::updateTriangle()
	{
		double triangleDegree = isOpen() ? 180_deg : 90_deg;
		if (not hasChildren()) return;
		setTriangle(Triangle(m_clickCenterPos, 15).rotated(triangleDegree));
		if (getTriangle().mouseOver())
		{
			Cursor::RequestStyle(CursorStyle::Hand);
		}
		if (getTriangle().leftClicked())
		{
			setOpen(!isOpen());
		}
	}

	void Node::drawLines() const
	{
		for (auto& child : getChildrenArray())
		{
			Vec2 p = m_rect.bl().movedBy(15, 0);
			Vec2 c = child->m_rect.leftCenter();
			Vec2 cross = { p.x, c.y };
			Line(p, cross).draw(1.0, m_lineColor);
			Line(cross, c).draw(1.0, m_lineColor);
		}
	}

	void Node::drawChildren() const
	{
		for (auto& child : getChildrenArray())
		{
			child->draw();
		}
	}

}





