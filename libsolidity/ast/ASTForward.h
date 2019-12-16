/*
	This file is part of solidity.

	solidity is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	solidity is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with solidity.  If not, see <http://www.gnu.org/licenses/>.
*/
/**
 * @author Christian <c@ethdev.com>
 * @date 2014
 * Forward-declarations of AST classes.
 */

#pragma once

#include <memory>
#include <iterator>
#include <string>
#include <utility>
#include <vector>

// Forward-declare all AST node types and related enums.

namespace langutil
{
enum class Token : unsigned int;
}

namespace dev
{
namespace solidity
{

class ASTNode;
class SourceUnit;
class PragmaDirective;
class ImportDirective;
class Declaration;
class CallableDeclaration;
class OverrideSpecifier;
class ContractDefinition;
class InheritanceSpecifier;
class UsingForDirective;
class StructDefinition;
class EnumDefinition;
class EnumValue;
class ParameterList;
class FunctionDefinition;
class VariableDeclaration;
class ModifierDefinition;
class ModifierInvocation;
class EventDefinition;
class MagicVariableDeclaration;
class TypeName;
class ElementaryTypeName;
class UserDefinedTypeName;
class FunctionTypeName;
class Mapping;
class ArrayTypeName;
class InlineAssembly;
class Statement;
class Block;
class PlaceholderStatement;
class IfStatement;
class TryCatchClause;
class TryStatement;
class BreakableStatement;
class WhileStatement;
class ForStatement;
class Continue;
class Break;
class Return;
class Throw;
class EmitStatement;
class VariableDeclarationStatement;
class ExpressionStatement;
class Expression;
class Conditional;
class Assignment;
class TupleExpression;
class UnaryOperation;
class BinaryOperation;
class FunctionCall;
class NewExpression;
class MemberAccess;
class IndexAccess;
class PrimaryExpression;
class Identifier;
class ElementaryTypeNameExpression;
class Literal;

class VariableScope;

// Used as pointers to AST nodes, to be replaced by more clever pointers, e.g. pointers which do
// not do reference counting but point to a special memory area that is completely released
// explicitly.
template <class T>
using ASTPointer = std::shared_ptr<T>;

using ASTString = std::string;

template <class T>
class ASTPtrVec {
private:
	using RawVec = std::vector<ASTPointer<T>>;
	using ConstRawVec = std::vector<ASTPointer<T const>>;
	using ConstPtrVec = ASTPtrVec<T const>;

public:
	static_assert(!std::is_const_v<T>);

	ASTPtrVec() : m_raw() {}

	/* implicit */ ASTPtrVec(RawVec const& _vec) : m_raw(_vec) {}

	/* implicit */ ASTPtrVec(RawVec&& _vec) : m_raw(std::move(_vec)) {}

	/* implicit */ operator RawVec const&() const { return get(); }
	/* implicit */ operator ConstRawVec const&() const { return m_const.get(); };
	/* implicit */ operator ConstPtrVec const&() const { return m_const; }

	RawVec const& get() const
	{
		return m_raw;
	}

private:
	RawVec m_raw;
	ASTPtrVec<T const> m_const = m_raw;
};

template <class T>
class ASTPtrVec<T const>
{
private:
    using RawVec = std::vector<ASTPointer<T const>>;
    using MutRawVec = std::vector<ASTPointer<T>>;

public:
	static_assert(!std::is_const_v<T>);

	ASTPtrVec() : m_raw() {}

	/* implicit */ ASTPtrVec(MutRawVec const& _vec) : m_raw(rawFromMut(_vec)) {}
	/* implicit */ ASTPtrVec(MutRawVec&& _vec) : m_raw(rawFromMut(std::move(_vec))) {}

	/* implicit */ ASTPtrVec(RawVec const& _vec) : m_raw(_vec) {}
	/* implicit */ ASTPtrVec(RawVec && _vec) : m_raw(std::move(_vec)) {}

	/* implicit */ operator RawVec const&() const { return get(); }

	RawVec const& get() const { return m_raw; }

private:
	static RawVec rawFromMut(MutRawVec const& _mutVec)
	{
		RawVec ret;
		ret.reserve(_mutVec.size());
		ret.insert(ret.end(), _mutVec.begin(), _mutVec.end());

		return ret;
	}

	static RawVec rawFromMut(MutRawVec&& _mutVec)
	{
		RawVec ret;
		ret.reserve(_mutVec.size());
		ret.insert(ret.end(), std::make_move_iterator(_mutVec.begin()), std::make_move_iterator(_mutVec.end()));

		return ret;
	}

    RawVec m_raw;
};
}
}
