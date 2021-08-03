#include "stdafx.h"
#include "Renderer.h"

#include <cstdint>

GameRenderer::GameRenderer()
{
	
}

void GameRenderer::loadScene()
{
	
}

void GameRenderer::onExit()
{
	
}

void GameRenderer::onLoad()
{
	
}

void GameRenderer::onFrameRender()
{
	
}

void GameRenderer::onResizeSwapChain(uint32_t width, uint32_t height)
{
    float h = (float)height;
    float w = (float)width;
	/*
    if (mpCamera)
    {
        mpCamera->setFocalLength(18);
        float aspectRatio = (w / h);
        mpCamera->setAspectRatio(aspectRatio);
    }

    mpRtOut = Texture::create2D(width, height, ResourceFormat::RGBA16Float, 1, 1, nullptr, Resource::BindFlags::UnorderedAccess | Resource::BindFlags::ShaderResource);
    */
}