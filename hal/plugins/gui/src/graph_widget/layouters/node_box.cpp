#include "gui/graph_widget/layouters/node_box.h"
#include "gui/graph_widget/graphics_factory.h"
#include "hal_core/netlist/netlist.h"
#include "gui/gui_globals.h"

uint qHash(const QPoint& p)
{
    uint retval = ( p.x() << 16);
    retval |= ( p.y() & 0xFFFF);
    return retval;
}

namespace hal
{

    NodeBox::NodeBox(const Node &n, int px, int py)
        : mNode(n), mX(px), mY(py),
          // GATE IO SPACING SHOULD BE CALCULATED HERE, FOR NOW IT IS JUST ASSUMED TO BE THE MINIMUM ACROSS THE BORD
          mInputPadding(sMinimumGateIoPadding),
          mOutputPadding(sMinimumGateIoPadding)
    {
        switch (type())
        {
        case Node::Module: {
            mItem = GraphicsFactory::createGraphicsModule(gNetlist->get_module_by_id(id()), 0);
            break;
        }
        case Node::Gate: {
            mItem = GraphicsFactory::createGraphicsGate(gNetlist->get_gate_by_id(id()), 0);
            break;
        }
        default:
            break;
        }
    }

    void NodeBox::setItemPosition(qreal xpos, qreal ypos)
    {
        Q_ASSERT(mItem);
        mItem->setPos(xpos,ypos);
    }

//------- QVector<NodeBox*> -----------------------
    NodeBoxes::~NodeBoxes()
    {
        for (NodeBox* nb : *this)
            delete nb;
    }

    void NodeBoxes::clearBoxes()
    {
        for (NodeBox* nb : *this)
            delete nb;
        clear();
        mGateHash.clear();
        mPointHash.clear();
        mNodeHash.clear();
        mItemHash.clear();
    }

    void NodeBoxes::addBox(const Node &nd, int px, int py)
    {
        NodeBox* nbox = new NodeBox(nd,px,py);

        mNodeHash.insert(nd,nbox);
        mPointHash.insert(QPoint(px,py),nbox);
        mItemHash.insert(nbox->item(),nbox);
        append(nbox);
        if (nbox->getNode().isGate())
        {
            Gate* g = gNetlist->get_gate_by_id(nbox->id());
            if (g) mGateHash.insert(g,nbox);
        }
        else if (nbox->getNode().isModule())
        {
            Module* m = gNetlist->get_module_by_id(nbox->id());
            if (!m) return;
            for (Gate* ig : m->get_gates(nullptr, true))
                if (ig) mGateHash.insert(ig,nbox);
        }
    }

    QSet<u32> NodeBoxes::filterNotInView(const QSet<u32>& gats) const
    {
        QSet<u32> retval;
        for (u32 gateId : gats)
        {
            if (!mNodeHash.value(Node(gateId,Node::Gate)))
                retval.insert(gateId);
        }
        return retval;
    }

    uint qHash(const Node &n)
    {
        uint retval = ( n.id() << 1);
        if (n.type() == Node::Module) ++retval;
        return retval;
    }

}
