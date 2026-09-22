#include "SimpleClippingNode.hpp"

using namespace geode::prelude;

namespace TailyUI {
    SimpleClippingNode* SimpleClippingNode::create(CCSize size) {
    	auto ret = new SimpleClippingNode();
        if (ret && ret->init(size)) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }

    bool SimpleClippingNode::init(CCSize size) {
        if (!CCClippingNode::init()) return false;

        m_stencil = CCDrawNode::create();

        setContentSize(size);
        setStencil(m_stencil);

        return true;
    }

    void SimpleClippingNode::setContentSize(const CCSize& size) {
        CCClippingNode::setContentSize(size);
        if (!m_stencil) return;
        
        bool shouldVisible = size.width > 0.0f && size.height > 0.0f;
        setVisible(shouldVisible);
        if (!shouldVisible) return;

        m_stencil->clear();

        CCPoint verts[] = {
            {0.0f, 0.0f},
            {size.width, 0.0f},
            {size.width, size.height},
            {0.0f, size.height}
        };

        m_stencil->drawPolygon(
            verts,
            4,
            {1.0f, 1.0f, 1.0f, 1.0f},
            0.0f,
            {1.0f, 1.0f, 1.0f, 1.0f}
        );
    }
}