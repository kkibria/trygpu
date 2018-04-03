
#include <vulkan/vulkan.h>

#include <stdio.h>
#include <stdlib.h>

class Device {
  VkDevice device;

public:
  operator VkDevice() { return device; }
};

class DescriptorSetLayouts {
    Device &device;

    void create() {
        VkDescriptorSetLayoutBinding descriptorSetLayoutBindings[2] = {
            {0, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1, VK_SHADER_STAGE_COMPUTE_BIT,
            0},
            {1, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1, VK_SHADER_STAGE_COMPUTE_BIT,
            0}};

        VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo = {
            VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO, 0, 0, 2,
            descriptorSetLayoutBindings};

        VkDescriptorSetLayout descriptorSetLayout;
        vkCreateDescriptorSetLayout(device, &descriptorSetLayoutCreateInfo, 0,
                                    &descriptorSetLayout);
    }


    void associate_buffers () {

		VkCommandPoolCreateInfo commandPoolCreateInfo = {
			VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
			0,
			0,
			queueFamilyIndex
		};

		VkDescriptorPoolSize descriptorPoolSize = {
			VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
			2
		};

		VkDescriptorPoolCreateInfo descriptorPoolCreateInfo = {
			VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
			0,
			0,
			1,
			1,
			&descriptorPoolSize
		};

		VkDescriptorPool descriptorPool;
		BAIL_ON_BAD_RESULT(vkCreateDescriptorPool(device, &descriptorPoolCreateInfo, 0, &descriptorPool));

		VkDescriptorSetAllocateInfo descriptorSetAllocateInfo = {
			VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
			0,
			descriptorPool,
			1,
			&descriptorSetLayout
		};

		VkDescriptorSet descriptorSet;
		BAIL_ON_BAD_RESULT(vkAllocateDescriptorSets(device, &descriptorSetAllocateInfo, &descriptorSet));

		VkDescriptorBufferInfo in_descriptorBufferInfo = {
			in_buffer,
			0,
			VK_WHOLE_SIZE
		};

		VkDescriptorBufferInfo out_descriptorBufferInfo = {
			out_buffer,
			0,
			VK_WHOLE_SIZE
		};

		VkWriteDescriptorSet writeDescriptorSet[2] = {
			{
				VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
				0,
				descriptorSet,
				0,
				0,
				1,
				VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
				0,
				&in_descriptorBufferInfo,
				0
			},
			{
				VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
				0,
				descriptorSet,
				1,
				0,
				1,
				VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
				0,
				&out_descriptorBufferInfo,
				0
			}
		};

		vkUpdateDescriptorSets(device, 2, writeDescriptorSet, 0, 0);

	    } 


}
