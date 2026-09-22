namespace TailyUI {
    class SimpleClippingNode : public cocos2d::CCClippingNode {
    public:
        static SimpleClippingNode* create(cocos2d::CCSize size);

        virtual void setContentSize(const cocos2d::CCSize& size) override;

    protected:
        cocos2d::CCDrawNode* m_stencil = nullptr;
        
        bool init(cocos2d::CCSize size);
        void updateStencil();
    };
}