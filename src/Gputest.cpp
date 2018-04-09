
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
};

class CommandBuffer {
    Device &device;
    VkCommandPool commandPool;
    VkCommandBuffer* pCommandBuffer;  // This should be an array
    uint32_t commandBufferCount;

    const VkCommandBuffer& operator[](uint32_t idx) const {
        if (idx < commandBufferCount) {
            return pCommandBuffer[idx];
        }
        throw "out of bound";
    }
    const uint32_t size() {return commandBufferCount;}

    // count should be greater than 0
    CommandBuffer(Device &device, uint32_t commandBufferCount, uint32_t queueFamilyIndex) 
        : device(device) {

        if (commandBufferCount == 0) {
            throw "size 0?";
        }

        pCommandBuffer = new VkCommandBuffer[commandBufferCount];
        this->commandBufferCount = commandBufferCount;
        create(queueFamilyIndex);
    }

    ~CommandBuffer() {
        delete[] pCommandBuffer;
    }

    void create(uint32_t queueFamilyIndex) {
		VkCommandPoolCreateInfo commandPoolCreateInfo = {
			VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
			0,
			0,
			queueFamilyIndex
		};


		BAIL_ON_BAD_RESULT(vkCreateCommandPool(device, &commandPoolCreateInfo, 0, &commandPool));

		VkCommandBufferAllocateInfo commandBufferAllocateInfo = {
			VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
			0,
			commandPool,
			VK_COMMAND_BUFFER_LEVEL_PRIMARY,
			commandBufferCount
		};
		
		BAIL_ON_BAD_RESULT(vkAllocateCommandBuffers(device, &commandBufferAllocateInfo, pCommandBuffer));

        // we probably should free up rest of the memeory from the pool
    }
};


class CommandBufferBind {

    CommandBufferBind() {

    VkCommandBufferBeginInfo commandBufferBeginInfo = {

      VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,

      0,

      VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,

      0

    };



    BAIL_ON_BAD_RESULT(vkBeginCommandBuffer(commandBuffer, &commandBufferBeginInfo));



    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, pipeline);



    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE,

      pipelineLayout, 0, 1, &descriptorSet, 0, 0);



    vkCmdDispatch(commandBuffer, bufferSize / sizeof(int32_t), 1, 1);



    BAIL_ON_BAD_RESULT(vkEndCommandBuffer(commandBuffer));





    }












};
