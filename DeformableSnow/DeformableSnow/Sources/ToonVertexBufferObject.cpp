#include "stdafx.h"
#include "ToonVertexBufferObject.h"
#include <glew/glew.h>
#include <iostream>

namespace Toon
{
	VertexBufferObject::VertexBufferObject(VertexBufferObject const& other) noexcept
	{

	}

	VertexBufferObject& VertexBufferObject::operator=(VertexBufferObject const& other) noexcept
	{
		if (this != &other)
		{

		}

		return *this;
	}

	VertexBufferObject::VertexBufferObject(VertexBufferObject&& other) noexcept
	{

	}

	VertexBufferObject& VertexBufferObject::operator=(VertexBufferObject&& other) noexcept
	{
		if (this != &other)
		{

		}

		return *this;
	}

	VertexBufferObject::~VertexBufferObject() noexcept
	{
		release();
	}

	bool VertexBufferObject::init() noexcept
	{
		glGenBuffers(1, &objectID);
		return true;
	}

	void VertexBufferObject::release() noexcept
	{
		if (objectID != 0U) glDeleteBuffers(1, &objectID);
		if (!descriptionArray.empty())
		{
			for (auto& description : descriptionArray)
				delete[] description.data;
			descriptionArray.clear();
		}
	}

	void VertexBufferObject::bind() const noexcept
	{
		glBindBuffer(GL_ARRAY_BUFFER, objectID);
	}

	void VertexBufferObject::unbind() const noexcept
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0U);
	}

	void VertexBufferObject::bindToGPU(bool _deleteAfterPush) noexcept
	{
		this->bind();
		long long prevOffset = 0LL;
		for (auto const& attribData : descriptionArray)
		{
			auto const& currentDesc = attribData.description;
			long long currentSize = static_cast<long long>(numberOfVertices) * currentDesc.stride;
			glBufferSubData(GL_ARRAY_BUFFER, prevOffset, currentSize, attribData.data);
			glEnableVertexAttribArray(attribData.index);
			glVertexAttribPointer(attribData.index, currentDesc.size, currentDesc.dataType, currentDesc.bNormalized, currentDesc.stride, reinterpret_cast<void*>(prevOffset));

			prevOffset += currentSize;
		}
		this->unbind();
 		if (_deleteAfterPush)
		{
			for (auto& description : descriptionArray)
				delete[] description.data;
			descriptionArray.clear();
		}
	}

	void VertexBufferObject::addVertexDescription(VertexAttribData _description) noexcept
	{
		descriptionArray.push_back(_description);
	}
};