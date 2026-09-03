#include "PointLight.h"
#include "imgui.h"

PointLight::PointLight( Graphics& gfx,float radius )
	:
	cbuf( gfx, 3u )
{
	Reset();
}

void PointLight::SpawnControlWindow() noexcept
{
	if( ImGui::Begin( "Light" ) )
	{
		ImGui::Text( "Position" );
		ImGui::SliderFloat( "X",&cbData.directionWS.x,-1.0f,1.0f,"%.01f" );
		ImGui::SliderFloat( "Y",&cbData.directionWS.y,-1.0f,1.0f,"%.01f" );
		ImGui::SliderFloat( "Z",&cbData.directionWS.z,-1.0f,1.0f,"%.01f" );
		
		ImGui::Text( "Intensity/Color" );
		ImGui::SliderFloat( "Intensity",&cbData.intensity,0.0f,5.0f,"%.2f");
		ImGui::ColorEdit3( "Diffuse Color",&cbData.color.x );
		
		if( ImGui::Button( "Reset" ) )
		{
			Reset();
		}
	}
	ImGui::End();
}

void PointLight::Reset() noexcept
{
	cbData = {
        { 0.0f, -1.0f, 0.5f },
        5.0f,
        { 1.0f, 1.0f, 1.0f },
        0.0f,
	};
}

void PointLight::Bind( Graphics& gfx,DirectX::FXMMATRIX view ) const noexcept
{
    auto dataCopy = cbData;

    const auto direction =
        DirectX::XMLoadFloat3(&dataCopy.directionWS);

    DirectX::XMStoreFloat3(
        &dataCopy.directionWS,
        DirectX::XMVector3Normalize(direction)
    );

    cbuf.Update(gfx, dataCopy);
    cbuf.Bind(gfx);
}
