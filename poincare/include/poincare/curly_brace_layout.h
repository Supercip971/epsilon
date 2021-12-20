#ifndef POINCARE_CURLY_BRACE_LAYOUT_NODE_H
#define POINCARE_CURLY_BRACE_LAYOUT_NODE_H

#include <poincare/brace_layout.h>
#include <poincare/serialization_helper.h>

namespace Poincare {

class CurlyBraceLayoutNode : public BraceLayoutNode {
public:
  // Dimensions
  static constexpr KDCoordinate k_curveHeight = 6;
  static constexpr KDCoordinate k_curveWidth = 5;
  static constexpr KDCoordinate k_centerHeight = 3;
  static constexpr KDCoordinate k_centerWidth = 3;
  static constexpr KDCoordinate k_barWidth = 1;
  // Margins
  static constexpr KDCoordinate k_horizontalExternalMargin = 1;
  static constexpr KDCoordinate k_horizontalInternalMargin = 1;
  static constexpr KDCoordinate k_verticalInternalMargin = 4;

  static constexpr KDCoordinate CurlyBraceWidth() { return k_horizontalExternalMargin + (k_centerWidth + k_curveWidth - k_barWidth) + k_horizontalInternalMargin; }

  using BraceLayoutNode::BraceLayoutNode;
  size_t size() const override { return sizeof(CurlyBraceLayoutNode); }
#if POINCARE_TREE_LOG
  void logNodeName(std::ostream & stream) const override {
    stream << "CurlyBraceLayout";
  }
#endif

protected:
  static KDCoordinate HeightGivenChildHeight(KDCoordinate childHeight) { return childHeight + k_verticalInternalMargin; }
  static KDCoordinate ChildHeightGivenLayoutHeight(KDCoordinate layoutHeight) { return layoutHeight - k_verticalInternalMargin; }
  static void RenderWithChildHeight(bool left, KDCoordinate childHeight, KDContext * ctx, KDPoint p, KDColor expressionColor, KDColor backgroundColor);

  KDSize computeSize() override { return KDSize(CurlyBraceWidth(), HeightGivenChildHeight(childHeight())); }
};

class LeftCurlyBraceLayoutNode final : public CurlyBraceLayoutNode {
public:
  static void RenderWithChildHeight(KDCoordinate childHeight, KDContext * ctx, KDPoint p, KDColor expressionColor, KDColor backgroundColor) { CurlyBraceLayoutNode::RenderWithChildHeight(true, childHeight, ctx, p, expressionColor, backgroundColor); }

  using CurlyBraceLayoutNode::CurlyBraceLayoutNode;
  Type type() const override { return Type::LeftCurlyBraceLayout; }
#if POINCARE_TREE_LOG
  void logNodeName(std::ostream & stream) const override {
    stream << "LeftCurlyBraceLayout";
  }
#endif
  int serialize(char * buffer, int bufferSize, Preferences::PrintFloatMode floatDisplayMode, int numberOfSignificantDigits) const override { return SerializationHelper::CodePoint(buffer, bufferSize, '{'); }

private:
  void render(KDContext * ctx, KDPoint p, KDColor expressionColor, KDColor backgroundColor, Layout * selectionStart = nullptr, Layout * selectionEnd = nullptr, KDColor selectionColor = KDColorRed) override { RenderWithChildHeight(CurlyBraceLayoutNode::ChildHeightGivenLayoutHeight(layoutSize().height()), ctx, p, expressionColor, backgroundColor); }
};

class LeftCurlyBraceLayout final : public Layout {
public:
  static LeftCurlyBraceLayout Builder() { return TreeHandle::FixedArityBuilder<LeftCurlyBraceLayout, LeftCurlyBraceLayoutNode>(); }
  LeftCurlyBraceLayout() = delete;
};

class RightCurlyBraceLayoutNode final : public CurlyBraceLayoutNode {
public:
  static void RenderWithChildHeight(KDCoordinate childHeight, KDContext * ctx, KDPoint p, KDColor expressionColor, KDColor backgroundColor) { CurlyBraceLayoutNode::RenderWithChildHeight(false, childHeight, ctx, p, expressionColor, backgroundColor); }

  using CurlyBraceLayoutNode::CurlyBraceLayoutNode;
  Type type() const override { return Type::RightCurlyBraceLayout; }
#if POINCARE_TREE_LOG
  void logNodeName(std::ostream & stream) const override {
    stream << "RightCurlyBraceLayout";
  }
#endif
  int serialize(char * buffer, int bufferSize, Preferences::PrintFloatMode floatDisplayMode, int numberOfSignificantDigits) const override { return SerializationHelper::CodePoint(buffer, bufferSize, '}'); }

private:
  void render(KDContext * ctx, KDPoint p, KDColor expressionColor, KDColor backgroundColor, Layout * selectionStart = nullptr, Layout * selectionEnd = nullptr, KDColor selectionColor = KDColorRed) override { RenderWithChildHeight(CurlyBraceLayoutNode::ChildHeightGivenLayoutHeight(layoutSize().height()), ctx, p, expressionColor, backgroundColor); }
};

class RightCurlyBraceLayout final : public Layout {
public:
  static RightCurlyBraceLayout Builder() { return TreeHandle::FixedArityBuilder<RightCurlyBraceLayout, RightCurlyBraceLayoutNode>(); }
  RightCurlyBraceLayout() = delete;
};

}

#endif